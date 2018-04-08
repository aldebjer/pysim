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
    for (SimulatableSystemInterface* s : d_ptr->subsystems) {
        std::vector<double*> subs_states = s->getStatePointers();
        std::copy(subs_states.begin(), subs_states.end(), std::back_inserter(states));
    }
    return states;
};

std::vector<double*> CompositeSystemImpl::getDerPointers() {
    std::vector<double*> ders;
    for (SimulatableSystemInterface* s : d_ptr->subsystems) {
        std::vector<double*> subs_ders = s->getDerPointers();
        std::copy(subs_ders.begin(), subs_ders.end(), std::back_inserter(ders));
    }
    return ders;
};


void CompositeSystemImpl::preSim()
{
    for (SimulatableSystemInterface* s : d_ptr->subsystems) {
        s->preSim();
    }
}

void CompositeSystemImpl::preStep() 
{
	for (SimulatableSystemInterface* s : d_ptr->subsystems) {
		s->preStep();
		s->copyoutputs();
		s->copystateoutputs();
	}

	copyinternaloutputs();
}

void CompositeSystemImpl::doStep(double time)
{
	copyinternalinputs();

    for (SimulatableSystemInterface* s : d_ptr->subsystems) {
        s->doStep(time);
        s->copyoutputs();
        s->copystateoutputs();
    }

	copyinternaloutputs();
}

void CompositeSystemImpl::postStep() {

	for (SimulatableSystemInterface* s : d_ptr->subsystems) {
		s->postStep();
	}

	copyinternaloutputs();
}

void CompositeSystemImpl::doStoreStep(double time) {
    d_ptr->storeHandler.doStoreStep(time);

    for (SimulatableSystemInterface* s : d_ptr->subsystems) {
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
    for (SimulatableSystemInterface* s : d_ptr->subsystems) {
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
    for (SimulatableSystemInterface* s : d_ptr->subsystems) {
        s->copystateoutputs();
    }
}

void CompositeSystemImpl::copyinternalinputs() {
	auto copyfunc = [](auto vi) {*(vi.second) = *(vi.first); };
	for_each(d_ptr->connected_inport_scalars.cbegin(), d_ptr->connected_inport_scalars.cend(), copyfunc);
	for_each(d_ptr->connected_inport_vectors.cbegin(), d_ptr->connected_inport_vectors.cend(), copyfunc);
	for_each(d_ptr->connected_inport_matrices.cbegin(), d_ptr->connected_inport_matrices.cend(), copyfunc);
}

void CompositeSystemImpl::copyinternaloutputs() {
	auto copyfunc = [](auto vi) {*(vi.second) = *(vi.first); };
	for_each(d_ptr->outports.connected_scalars.cbegin(), d_ptr->outports.connected_scalars.cend(), copyfunc);
	for_each(d_ptr->outports.connected_vectors.cbegin(), d_ptr->outports.connected_vectors.cend(), copyfunc);
	for_each(d_ptr->outports.connected_matrices.cbegin(), d_ptr->outports.connected_matrices.cend(), copyfunc);
}

void CompositeSystemImpl::copyoutputs() {
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
    StoreHandler* shp = &d_ptr->storeHandler;
    if (shp->checkAndStore(name, outputs)) return;
    if (shp->checkAndStore(name, inputs)) return;

    char errmsg[50];
    snprintf(errmsg, sizeof(errmsg), "Could not store: %s, no such variable", name);
    throw std::invalid_argument(errmsg);

}

StoreHandler* CompositeSystemImpl::getStoreHandlerP() {
    return &(d_ptr->storeHandler);
}

void CompositeSystemImpl::add_subsystem(CommonSystemImpl* subsystem, string name)
{
    if (subsystem->getDiscrete()) {
        throw std::invalid_argument("Discrete systems not supported as subsystems");
    }
    d_ptr->subsystems_map[name] = subsystem;
    d_ptr->subsystems.push_back(subsystem);
}

void CompositeSystemImpl::add_composite_subsystem(CompositeSystemImpl* subsystem, string name)
{
    if (subsystem->getDiscrete()) {
        throw std::invalid_argument("Discrete systems not supported as subsystems");
    }
    d_ptr->subsystems_map[name] = subsystem;
    d_ptr->subsystems.push_back(subsystem);
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
    outputs.d_ptr->matrices[name] = d_ptr->outports.matrices.back().get();
    outputs.d_ptr->descriptions[name] = description;
}

void CompositeSystemImpl::connect_port_in(std::string portname, CommonSystemImpl* subsystem, std::string subsystem_input) {

    if (inputs.d_ptr->scalars.count(portname) > 0) {
        double* port_p = inputs.d_ptr->scalars.at(portname);
        double* sub_input_p = subsystem->inputs.d_ptr->scalars.at(subsystem_input);
        auto p = std::make_pair(port_p, sub_input_p);
        d_ptr->connected_inport_scalars.push_back(p);
		*port_p = *sub_input_p;
    } else if (inputs.d_ptr->vectors.count(portname) > 0) {
        pysim::vector* port_p = inputs.d_ptr->vectors.at(portname);
        pysim::vector* sub_input_p = subsystem->inputs.d_ptr->vectors.at(subsystem_input);
        auto p = std::make_pair(port_p, sub_input_p);
        d_ptr->connected_inport_vectors.push_back(p);
		*port_p = *sub_input_p;
    } else if (inputs.d_ptr->matrices.count(portname) > 0) {
        Eigen::MatrixXd* port_p = inputs.d_ptr->matrices.at(portname);
        Eigen::MatrixXd* sub_input_p = subsystem->inputs.d_ptr->matrices.at(subsystem_input);
        auto p = std::make_pair(port_p, sub_input_p);
        d_ptr->connected_inport_matrices.push_back(p);
		*port_p = *sub_input_p;
    } else {
        throw std::invalid_argument("Port not created");
    }
}

void CompositeSystemImpl::connect_port_in_composite(std::string portname, CompositeSystemImpl* subsystem, std::string subsystem_input) {
    
        if (inputs.d_ptr->scalars.count(portname) > 0) {
            double* port_p = inputs.d_ptr->scalars.at(portname);
            double* sub_input_p = subsystem->inputs.d_ptr->scalars.at(subsystem_input);
            auto p = std::make_pair(port_p, sub_input_p);
            d_ptr->connected_inport_scalars.push_back(p);
			*port_p = *sub_input_p;
        } else if (inputs.d_ptr->vectors.count(portname) > 0) {
            pysim::vector* port_p = inputs.d_ptr->vectors.at(portname);
            pysim::vector* sub_input_p = subsystem->inputs.d_ptr->vectors.at(subsystem_input);
            auto p = std::make_pair(port_p, sub_input_p);
            d_ptr->connected_inport_vectors.push_back(p);
			*port_p = *sub_input_p;
        } else if (inputs.d_ptr->matrices.count(portname) > 0) {
            Eigen::MatrixXd* port_p = inputs.d_ptr->matrices.at(portname);
            Eigen::MatrixXd* sub_input_p = subsystem->inputs.d_ptr->matrices.at(subsystem_input);
            auto p = std::make_pair(port_p, sub_input_p);
            d_ptr->connected_inport_matrices.push_back(p);
			*port_p = *sub_input_p;
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
				*port_p = *sub_output_p;
                return;
            }
        }
        throw std::invalid_argument("Could not find matching state, der, or output to connect from");
    } else if (outputs.d_ptr->vectors.count(portname) > 0) {
        pysim::vector* port_p = outputs.d_ptr->vectors.at(portname);

		std::vector<std::map<std::string, pysim::vector* >*> v;
		v.push_back(&subsystem->outputs.d_ptr->vectors);
		v.push_back(&subsystem->states.d_ptr->vectors);
		v.push_back(&subsystem->ders.d_ptr->vectors);
		for (auto item : v) {
			if (item->count(subsystem_output)) {
				pysim::vector* sub_output_p = item->at(subsystem_output);
				auto p = std::make_pair(sub_output_p, port_p);
				d_ptr->outports.connected_vectors.push_back(p);
				*port_p = *sub_output_p;
				return;
			}
		}
		throw std::invalid_argument("Could not find matching state, der, or output to connect from");
    } else if (outputs.d_ptr->matrices.count(portname) > 0) {
        Eigen::MatrixXd* port_p = outputs.d_ptr->matrices.at(portname);

		std::vector<std::map<std::string, Eigen::MatrixXd* >*> v;
		v.push_back(&subsystem->outputs.d_ptr->matrices);
		v.push_back(&subsystem->states.d_ptr->matrices);
		v.push_back(&subsystem->ders.d_ptr->matrices);
		for (auto item : v) {
			if (item->count(subsystem_output)) {
				Eigen::MatrixXd* sub_output_p = item->at(subsystem_output);
				auto p = std::make_pair(sub_output_p, port_p);
				d_ptr->outports.connected_matrices.push_back(p);
				*port_p = *sub_output_p;
				return;
			}
		}
		throw std::invalid_argument("Could not find matching state, der, or output to connect from");
    } else {
        throw std::invalid_argument("Port not created");
    }
}

void CompositeSystemImpl::connect_port_out_composite(std::string portname, CompositeSystemImpl* subsystem, std::string subsystem_output) {
    
        if (outputs.d_ptr->scalars.count(portname) > 0) {
            double* port_p = outputs.d_ptr->scalars.at(portname);

			if (subsystem->outputs.d_ptr->scalars.count(subsystem_output) > 0) {
				double* sub_output_p = subsystem->outputs.d_ptr->scalars.at(subsystem_output);
				auto p = std::make_pair(sub_output_p, port_p);
				d_ptr->outports.connected_scalars.push_back(p);
				*port_p = *sub_output_p;
			}
			else {
				throw std::invalid_argument("Could not find matching state, der, or output to connect from");
			}
   
        } else if (outputs.d_ptr->vectors.count(portname) > 0) {
            pysim::vector* port_p = outputs.d_ptr->vectors.at(portname);

			if (subsystem->outputs.d_ptr->vectors.count(subsystem_output) > 0) {
				pysim::vector* sub_output_p = subsystem->outputs.d_ptr->vectors.at(subsystem_output);
				auto p = std::make_pair(sub_output_p, port_p);
				d_ptr->outports.connected_vectors.push_back(p);
				*port_p = *sub_output_p;
			}
			else {
				throw std::invalid_argument("Could not find matching state, der, or output to connect from");
			}
        } else if (outputs.d_ptr->matrices.count(portname) > 0) {
            Eigen::MatrixXd* port_p = outputs.d_ptr->matrices.at(portname);

			if (subsystem->outputs.d_ptr->matrices.count(subsystem_output) > 0) {
				Eigen::MatrixXd* sub_output_p = subsystem->outputs.d_ptr->matrices.at(subsystem_output);
				auto p = std::make_pair(sub_output_p, port_p);
				d_ptr->outports.connected_matrices.push_back(p);
				*port_p = *sub_output_p;
			}
			else {
				throw std::invalid_argument("Could not find matching state, der, or output to connect from");
			}
        } else {
            throw std::invalid_argument("Port not created");
        }
    }

} //End namespace pysim
