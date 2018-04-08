#pragma once
#include <vector>

#include "CommonSystemImpl.hpp"

namespace pysim{

class  CythonSystemImpl :
    public CommonSystemImpl
{
public:
    CythonSystemImpl();
    ~CythonSystemImpl(){};

    void preSim(){};
	void preStep();
    void doStep(double time);
	void postStep();

    double getNextUpdateTime();

    bool do_comparison();



    void* sysp;

    //Python Interface
    void add_input(std::string name, size_t length);
    void add_input_scalar(std::string name, std::string desc);
    void add_input_vector(std::string name, size_t rows, std::string desc);
    void add_input_matrix(std::string name, size_t rows, size_t cols, std::string desc);
    void add_output(std::string name, size_t length);
    void add_output_scalar(std::string name, std::string desc);
    void add_output_vector(std::string name, size_t rows, std::string desc);
    void add_output_matrix(std::string name, size_t rows, size_t cols, std::string desc);
    void add_state(std::string statename, std::string dername, size_t length);
    void add_state_scalar(std::string statename, std::string dername, std::string desc);
    void add_state_vector(std::string statename, std::string dername, size_t rows, std::string desc);
    void add_state_matrix(std::string statename, std::string dername, size_t rows, size_t cols, std::string desc);
};

}
