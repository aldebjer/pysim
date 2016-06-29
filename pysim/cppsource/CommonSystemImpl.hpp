#pragma once
#include <vector>
#include <map>
#include <memory>

#include "SimulatableSystem.hpp"

struct CommonSystemImplPrivate;

class  CommonSystemImpl :
    public SimulatableSystem
{
public:
    CommonSystemImpl();
    virtual ~CommonSystemImpl();

    void copyoutputs();
    void copystateoutputs();

    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();

    void doStoreStep(double time);
    void store(char* name);

    //Input handling
    std::vector<std::string> getScalarInputNames();
    std::vector<std::string> getInputVectorNames();
    std::vector<double> getInputVector(char* name);
    void setInputVector(char* name, std::vector<double> value);
    std::map<std::string, std::string> getInputDescriptionMap();

    //Output handling
    std::vector<std::string> getOutputNames();
    std::vector<std::string> getOutputVectorNames();
    double getOutput(char* name);
    std::vector<double> getOutputVector(char* name);
    void setOutputVector(char* name, std::vector<double> value);
    std::map<std::string, std::string> getOutputDescriptionMap();

    //State handling
    std::vector<std::string> getScalarStatetNames();
    std::vector<std::string> getStateVectorNames();
    double getState(char* name);
    std::vector<double> getStateVector(char* name);
    void setStateVector(char* name, std::vector<double> value);
    std::map<std::string, std::string> getStateDescriptionMap();

    //Der handling
    std::vector<std::string> getScalarDerNames();
    std::vector<std::string> getDerVectorNames();
    double getDer(char* name);
    std::vector<double> getDerVector(char* name);
    void setDerVector(char* name, std::vector<double> value);
    std::map<std::string, std::string> getDerDescriptionMap();

    void connect(char* outputname, CommonSystemImpl* inputsys, char* inputname);

protected:
    std::unique_ptr<CommonSystemImplPrivate> d_ptr;

};

