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
    ConnectionHandler(
        Variable* inputp,
        Variable* outputp, 
        Variable* statep = nullptr, 
        Variable* derp = nullptr);
    virtual ~ConnectionHandler();

    void copyinputs();
    void copyoutputs();
    void copystateoutputs();

    void connect(char* outputname, CommonSystemImpl* inputsys, char* inputname);

private:
    std::unique_ptr<ConnectionHandlerPrivate> d_ptr;

};

}
