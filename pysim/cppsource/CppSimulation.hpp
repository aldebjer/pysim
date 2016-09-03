//   Copyright (c) 2014-2016 SSPA Sweden AB

#pragma once

#include <vector>

#include "SimulatableSystem.hpp"

namespace pysim{

class Simulation {
 public:
    Simulation(void);
    ~Simulation(void);

    void simulate(double duration, double dt, char* solvername,
                  double abs_err, double rel_err, bool dense_output);

    void observer(const std::vector<double> &state, double time);
    void addSystem(SimulatableSystemInterface* system);
    double getCurrentTime() { return currentTime; }

 protected:
    void prepare_first_sim();
    void setup_odeint(double endtime, double dt, char* solvername,
                      double abs_err, double rel_err, bool dense_output);
    void doGenericStep(const std::vector< double > &state,
                       std::vector< double > &der,
                       double time);

    std::vector<SimulatableSystemInterface*> systems;
    std::vector<SimulatableSystemInterface*> discreteSystems;

    std::vector<double*> states;
    std::vector<double*> ders;

    double currentTime;
};

}
