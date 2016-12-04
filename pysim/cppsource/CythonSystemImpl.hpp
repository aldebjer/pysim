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
    void doStep(double time);

    double getNextUpdateTime();

    bool do_comparison();



    void* sysp;

    //Python Interface
    void add_input(std::string name, size_t length);
    void add_input_scalar(std::string name);
    void add_input_vector(std::string name, size_t rows);
    void add_input_matrix(std::string name, size_t rows, size_t cols);
    void add_output(std::string name, size_t length);
    void add_output_scalar(std::string name, std::string desc);
    void add_output_vector(std::string name, size_t rows, std::string desc);
    void add_output_matrix(std::string name, size_t rows, size_t cols, std::string desc);
    void add_state(std::string statename, std::string dername, size_t length);
    void add_state(std::string statename, std::string dername, size_t rows, size_t cols);
};

}
