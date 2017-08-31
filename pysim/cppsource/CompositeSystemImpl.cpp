#include "CompositeSystemImpl_p.hpp"

#include <map>

#include "CommonSystemImpl_p.hpp"

#include "Variable_p.hpp"

#include "PysimTypes.hpp"

#include <iostream>
#include <string>

#include "StoreHandler.hpp"

using std::string;

namespace pysim{


CompositeSystemImpl::CompositeSystemImpl():
    connectionHandler(&outputs),
    d_ptr(new CompositeSystemImplPrivate())
{
}
CompositeSystemImpl::~CompositeSystemImpl()
{
}

//////////////////////////////////////////////////////////////////////////
//      Inherited from Simulatable System
//////////////////////////////////////////////////////////////////////////

std::vector<double*> CompositeSystemImpl::getStatePointers() {
    std::vector<double*> states;
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        std::vector<double*> subs_states = s->getStatePointers();
        std::copy(subs_states.begin(), subs_states.end(), std::back_inserter(states));
    }
    return states;
};

std::vector<double*> CompositeSystemImpl::getDerPointers() {
    std::vector<double*> ders;
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        std::vector<double*> subs_ders = s->getDerPointers();
        std::copy(subs_ders.begin(), subs_ders.end(), std::back_inserter(ders));
    }
    return ders;
};


void CompositeSystemImpl::preSim()
{
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->preSim();
    }
}

void CompositeSystemImpl::doStep(double time)
{
    auto copyfunc = [](auto vi) {*(vi.second) = *(vi.first); };
    for_each(d_ptr->connected_inport_scalars.cbegin(), d_ptr->connected_inport_scalars.cend(), copyfunc);
    for_each(d_ptr->connected_inport_vectors.cbegin(), d_ptr->connected_inport_vectors.cend(), copyfunc);
    for_each(d_ptr->connected_inport_matrices.cbegin(), d_ptr->connected_inport_matrices.cend(), copyfunc);

    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->doStep(time);
        s->copyoutputs();
        s->copystateoutputs();
    }
}

void CompositeSystemImpl::doStoreStep(double time) {
    d_ptr->storeHandler.doStoreStep(time);

    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->doStoreStep(time);
    }

}

double CompositeSystemImpl::getNextUpdateTime() 
{
    return 0;
}

bool CompositeSystemImpl::do_comparison()
{
    bool comparison_trigged = false;
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        comparison_trigged = comparison_trigged || s->do_comparison();
    }
    return comparison_trigged;
}

////////////////////////////////////
//
//       Connections
//
////////////////////////////////////

void CompositeSystemImpl::copystateoutputs() {
    for (auto syst: d_ptr->subsystems_common) {
        syst->copystateoutputs();
    }
}

void CompositeSystemImpl::copyoutputs() {

    auto copyfunc = [](auto vi) {*(vi.second) = *(vi.first); };
    for_each(d_ptr->outports.connected_scalars.cbegin(), d_ptr->outports.connected_scalars.cend(), copyfunc);
    for_each(d_ptr->outports.connected_vectors.cbegin(), d_ptr->outports.connected_vectors.cend(), copyfunc);
    for_each(d_ptr->outports.connected_matrices.cbegin(), d_ptr->outports.connected_matrices.cend(), copyfunc);

    connectionHandler.copyoutputs();
}

/////////////////////////////////////////////////////////////////////
//
//         Python Interface
//
/////////////////////////////////////////////////////////////////////


//Put the state, der, input or output named "name" in the vector of pointers 
//to be stored. If none with "name" is found the function raises an invalid_argument
//exception.
void  CompositeSystemImpl::store(char* name) {
    if (inputs.d_ptr->scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, inputs.d_ptr->scalars.at(name));
    } else if (outputs.d_ptr->scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, outputs.d_ptr->scalars.at(name));
    } else if (inputs.d_ptr->vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, inputs.d_ptr->vectors.at(name));
    } else if (outputs.d_ptr->vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, outputs.d_ptr->vectors.at(name));
    } else {
        char errmsg[50];
        snprintf(errmsg, sizeof(errmsg), "Could not store: %s, no such variable", name);
        throw std::invalid_argument(errmsg);
    }
}

StoreHandler* CompositeSystemImpl::getStoreHandlerP() {
    return &(d_ptr->storeHandler);
}

void CompositeSystemImpl::add_subsystem(CommonSystemImpl* subsystem, string name)
{
    if (subsystem->getDiscrete()) {
        throw std::invalid_argument("Discrete systems not supported as subsystems");
    }
    d_ptr->subsystems_common_map[name] = subsystem;
    d_ptr->subsystems_common.push_back(subsystem);
}

void CompositeSystemImpl::add_subsystem(CompositeSystemImpl* subsystem, string name)
{
    if (subsystem->getDiscrete()) {
        throw std::invalid_argument("Discrete systems not supported as subsystems");
    }
    d_ptr->subsystems_composite_map[name] = subsystem;
    d_ptr->subsystems_composite.push_back(subsystem);
}

void CompositeSystemImpl::add_scalar_port_in(std::string name, double initial_value, std::string description) {
    std::unique_ptr<double> ptr(new double(initial_value));
    d_ptr->scalar_inports.push_back(std::move(ptr));
    inputs.d_ptr->scalars[name] = d_ptr->scalar_inports.back().get();
    inputs.d_ptr->descriptions[name] = description;
}


void CompositeSystemImpl::add_vector_inport(std::string name, std::vector<double> initial_value, std::string description) {
    pysim::vector pyvec(initial_value.size());
    std::copy(initial_value.begin(), initial_value.end(), pyvec.begin());
    
    std::unique_ptr<pysim::vector> ptr(new pysim::vector(pyvec));
    d_ptr->vector_inports.push_back(std::move(ptr));
    inputs.d_ptr->vectors[name] = d_ptr->vector_inports.back().get();
    inputs.d_ptr->descriptions[name] = description;
}


void CompositeSystemImpl::add_matrix_inport(std::string name, std::vector<std::vector<double>> initial_value, std::string description) {
    size_t rows = initial_value.size();
    size_t columns = initial_value.front().size();
    Eigen::MatrixXd m(rows, columns);
    for (size_t i = 0; i < rows; ++i) {
        if (initial_value[i].size() != columns) {
            throw std::invalid_argument("Matrix rows has different number of columns");
        }
        for (size_t j = 0; j < columns; ++j) {
            m(i, j) = initial_value[i][j];
        }
    }

    std::unique_ptr<Eigen::MatrixXd> ptr(new Eigen::MatrixXd(m));
    d_ptr->matrix_inports.push_back(std::move(ptr));
    inputs.d_ptr->matrices[name] = d_ptr->matrix_inports.back().get();
    inputs.d_ptr->descriptions[name] = description;
}

////Outports

void CompositeSystemImpl::add_outport(std::string name, double initial_value, std::string description) {
    std::unique_ptr<double> ptr(new double(0));
    d_ptr->outports.scalars.push_back(std::move(ptr));
    outputs.d_ptr->scalars[name] = d_ptr->outports.scalars.back().get();
    outputs.d_ptr->descriptions[name] = description;
}


void CompositeSystemImpl::add_outport(std::string name, std::vector<double> initial_value, std::string description) {
    pysim::vector pyvec(initial_value.size());
    std::copy(initial_value.begin(), initial_value.end(), pyvec.begin());

    std::unique_ptr<pysim::vector> ptr(new pysim::vector(pyvec));
    d_ptr->outports.vectors.push_back(std::move(ptr));
    outputs.d_ptr->vectors[name] = d_ptr->outports.vectors.back().get();
    outputs.d_ptr->descriptions[name] = description;
}


void CompositeSystemImpl::add_outport(std::string name, std::vector<std::vector<double>> initial_value, std::string description) {
    size_t rows = initial_value.size();
    size_t columns = initial_value.front().size();
    Eigen::MatrixXd m(rows, columns);
    for (size_t i = 0; i < rows; ++i) {
        if (initial_value[i].size() != columns) {
            throw std::invalid_argument("Matrix rows has different number of columns");
        }
        for (size_t j = 0; j < columns; ++j) {
            m(i, j) = initial_value[i][j];
        }
    }

    std::unique_ptr<Eigen::MatrixXd> ptr(new Eigen::MatrixXd(m));
    d_ptr->outports.matrices.push_back(std::move(ptr));
    outputs.d_ptr->matrices[name] = d_ptr->matrix_inports.back().get();
    outputs.d_ptr->descriptions[name] = description;
}

void CompositeSystemImpl::connect_port_in(std::string portname, CommonSystemImpl* subsystem, std::string subsystem_input) {

    if (inputs.d_ptr->scalars.count(portname) > 0) {
        double* port_p = inputs.d_ptr->scalars.at(portname);
        double* sub_input_p = subsystem->inputs.d_ptr->scalars.at(subsystem_input);
        auto p = std::make_pair(port_p, sub_input_p);
        d_ptr->connected_inport_scalars.push_back(p);
    } else if (inputs.d_ptr->vectors.count(portname) > 0) {
        pysim::vector* port_p = inputs.d_ptr->vectors.at(portname);
        pysim::vector* sub_input_p = subsystem->inputs.d_ptr->vectors.at(subsystem_input);
        auto p = std::make_pair(port_p, sub_input_p);
        d_ptr->connected_inport_vectors.push_back(p);
    } else if (inputs.d_ptr->matrices.count(portname) > 0) {
        Eigen::MatrixXd* port_p = inputs.d_ptr->matrices.at(portname);
        Eigen::MatrixXd* sub_input_p = subsystem->inputs.d_ptr->matrices.at(subsystem_input);
        auto p = std::make_pair(port_p, sub_input_p);
        d_ptr->connected_inport_matrices.push_back(p);
    } else {
        throw std::invalid_argument("Port not created");
    }
}



void CompositeSystemImpl::connect_port_out(std::string portname, CommonSystemImpl* subsystem, std::string subsystem_output) {

    if (outputs.d_ptr->scalars.count(portname) > 0) {
        double* port_p = outputs.d_ptr->scalars.at(portname);

        std::vector<std::map<std::string, double* >*> v;
        v.push_back(&subsystem->outputs.d_ptr->scalars);
        v.push_back(&subsystem->states.d_ptr->scalars);
        v.push_back(&subsystem->ders.d_ptr->scalars);
        for (auto item : v) {
            if (item->count(subsystem_output)) {
                double* sub_output_p = item->at(subsystem_output);
                auto p = std::make_pair(sub_output_p, port_p);
                d_ptr->outports.connected_scalars.push_back(p);
                return;
            }
        }
        throw std::invalid_argument("Could not find matching state, der, or output to connect from");
    } else if (outputs.d_ptr->vectors.count(portname) > 0) {
        pysim::vector* port_p = outputs.d_ptr->vectors.at(portname);
        pysim::vector* sub_output_p = subsystem->outputs.d_ptr->vectors.at(subsystem_output);
        auto p = std::make_pair(sub_output_p, port_p);
        d_ptr->outports.connected_vectors.push_back(p);
    } else if (outputs.d_ptr->matrices.count(portname) > 0) {
        Eigen::MatrixXd* port_p = outputs.d_ptr->matrices.at(portname);
        Eigen::MatrixXd* sub_output_p = subsystem->outputs.d_ptr->matrices.at(subsystem_output);
        auto p = std::make_pair(sub_output_p, port_p);
        d_ptr->outports.connected_matrices.push_back(p);
    } else {
        throw std::invalid_argument("Port not created");
    }
}

} //End namespace pysim
