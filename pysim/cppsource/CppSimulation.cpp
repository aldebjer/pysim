#include "CppSimulation.hpp"

#include <boost/numeric/odeint.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include <exception>

namespace pysim{

struct EarlyBreakException : public std::exception {
    virtual const char* what() const throw() {
        return "A comparison resulted in an early break of the simulation";
    }
}earlybreak;

Simulation::Simulation(void)
    :currentTime(0) {
}


Simulation::~Simulation(void) {
}

void Simulation::doGenericStep(const std::vector< double > &state,
                               std::vector< double > &der,
                               double time) {
    // Copy the state variables to the actual systems
    auto si = states.cbegin();
    for (auto i = state.cbegin(); i != state.cend(); ++i) {
        **si++ = *i;
    }

    // Copy state outputs from all systems to their respective inputs
    // Since they are states, and constant input to this function this
    // is done before the timestep calulations.
    for ( auto syst = systems.begin(); syst != systems.end(); ++syst ) {
		(*syst)->preStep();
        (*syst)->copystateoutputs();
		(*syst)->copyoutputs();
    }

    // Do the time step for all systems, and copy the variable
    // outputs after the time step.
    for ( auto syst = systems.begin(); syst != systems.end(); ++syst ) {
        (*syst)->doStep(time);
        (*syst)->copyoutputs();
    }

    // Copy the systems derivate variables to the
    // solvers (the input to this function).
    auto di = der.begin();
    for ( auto i = ders.cbegin(); i != ders.cend(); ++i ) {
        *(di++) = **i;
    }
}

void Simulation::observer(const std::vector<double> &state, double time) {
    currentTime = time;

    // Copy the state variables to the actual systems
    auto si = states.cbegin();
    for (auto i = state.cbegin(); i != state.cend(); ++i) {
        **si++ = *i;
    }

    for ( auto syst = systems.cbegin(); syst != systems.end(); ++syst ) {
        (*syst)->postStep();
        (*syst)->doStoreStep(time);
    }
    for (   auto syst = discreteSystems.cbegin();
            syst != discreteSystems.end();
            ++syst) {
        (*syst)->postStep();
        (*syst)->doStoreStep(time);
    }

    bool compare_break = false;
    for (auto syst = systems.cbegin(); syst != systems.end(); ++syst) {
        if ((*syst)->do_comparison()) {
            compare_break = true;
        }
    }
    if (compare_break) {
        throw earlybreak;
    }
}


void Simulation::prepare_first_sim() {
    // Copy system states to this objects list
    for ( auto syst = systems.cbegin(); syst != systems.end(); ++syst ) {
        (*syst)->preSim();

        std::vector<double*> sp = (*syst)->getStatePointers();
        std::copy(sp.cbegin(), sp.cend(), std::back_inserter(states));

        std::vector<double*> dp = (*syst)->getDerPointers();
        std::copy(dp.cbegin(), dp.cend(), std::back_inserter(ders));

        if (ders.size() != states.size()) {
            throw std::runtime_error("Unequal states and ders");
        }

    }
    for (   auto syst = discreteSystems.cbegin();
            syst != discreteSystems.end();
            ++syst) {
        (*syst)->preSim();
    }
}

bool myfn(SimulatableSystemInterface* i, SimulatableSystemInterface* j) {
    return i->getNextUpdateTime() < j->getNextUpdateTime();
}

void Simulation::simulate(double duration,
                          double dt,
                          char* solvername,
                          double abs_err,
                          double rel_err,
                          bool dense_output) {
    if (currentTime == 0.0) {
        prepare_first_sim();
    }
    double endtime = currentTime + duration;

    if (discreteSystems.size() > 0) {
        auto si = std::min_element(discreteSystems.begin(),
                                   discreteSystems.end(),
                                   myfn);
        double inter_endtime = (*si)->getNextUpdateTime();
        while (inter_endtime < endtime) {
            if (inter_endtime > 0) {
                setup_odeint(inter_endtime, dt, solvername,
                             abs_err, rel_err, dense_output);
            }
            do {
                (*si)->doStep(currentTime);
                std::vector<double*> states = (*si)->getStatePointers();
                std::vector<double*> ders = (*si)->getDerPointers();

                //Copy states to der
                auto state_iter = states.begin();
                auto der_iter = ders.begin();
                while (state_iter != states.end()) {
                    **state_iter++ = **der_iter++;
                }

                (*si)->copystateoutputs();
                (*si)->copyoutputs();
                si = std::min_element(discreteSystems.begin(),
                                      discreteSystems.end(),
                                      myfn);
            } while ((*si)->getNextUpdateTime() == inter_endtime);

            si = std::min_element(discreteSystems.begin(),
                                  discreteSystems.end(),
                                  myfn);
            inter_endtime = (*si)->getNextUpdateTime();
        }
    }
    setup_odeint(endtime, dt, solvername, abs_err, rel_err, dense_output);
}

void Simulation::setup_odeint(double endtime,
                              double dt,
                              char* solvername,
                              double abs_err,
                              double rel_err,
                              bool dense_output) {
    using namespace boost::numeric::odeint;
    using std::vector;

    namespace pl = std::placeholders;
    auto ff = std::bind(&Simulation::doGenericStep,
                        this,
                        pl::_1, pl::_2, pl::_3);
    auto sf = std::bind(&Simulation::observer, this, pl::_1, pl::_2);

    vector< double > state;
    for ( auto i = states.cbegin(); i != states.cend(); ++i ) {
        state.push_back(**i);
    }
    try {
        if (strcmp(solvername, "rk4") == 0) {
            runge_kutta4<vector< double > > stepper;
            integrate_const(stepper, ff, state, currentTime, endtime, dt, sf);
        } else if (strcmp(solvername, "ck54") == 0) {
            typedef runge_kutta_cash_karp54<vector<double> > error_stepper_type;
            auto cs = make_controlled <error_stepper_type>(abs_err, rel_err);
            integrate_adaptive(cs, ff, state, currentTime, endtime, dt, sf);
        } else if (strcmp(solvername, "dp5") == 0) {
            typedef runge_kutta_dopri5<vector<double> > error_stepper_type;
            if (dense_output) {
                auto cs = make_dense_output(abs_err, rel_err, error_stepper_type());
                integrate_const(cs, ff, state, currentTime, endtime, dt, sf);
            } else {
                auto cs = make_controlled <error_stepper_type>(abs_err, rel_err);
                integrate_adaptive(cs, ff, state, currentTime, endtime, dt, sf);
            }
        } else {
            char errmsg[50];
            snprintf(errmsg, sizeof(errmsg), "Unknown solver: %s", solvername);
            throw std::invalid_argument(errmsg);
        }
    } catch (EarlyBreakException &eb) {
        printf("%s\n",eb.what());
    }
}


void Simulation::addSystem(SimulatableSystemInterface* system) {
    if (system->getDiscrete()) {
        discreteSystems.push_back(system);
    } else {
        systems.push_back(system);
    }
}

}
