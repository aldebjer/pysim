//   Copyright (c) 2014-2016 SSPA Sweden AB
#pragma once

#include <vector>
#include <map>
#include <string>


#include "PysimTypes.hpp"
#include "Variable.hpp"

namespace pysim {

struct ConnectionHandlerPrivate;
class CommonSystemImpl;
class CompositeSystemImpl;

class ConnectionHandler{
public:
    ConnectionHandler(Variable* outputp, Variable* statep = nullptr, Variable* derp = nullptr);
    virtual ~ConnectionHandler();

    void copyoutputs();
    void copystateoutputs();

    template <typename T>
    void connect(char* outputname, T* inputsys, char* inputname);

    template <typename T>
    void connect(char* outputname, T* inputsys, char* inputname, int output_index);

protected:

    template <typename T>
    bool check_input(std::map<std::string, T > input, char* inputname, char* outputname);


private:
    std::unique_ptr<ConnectionHandlerPrivate> d_ptr;

};

}
