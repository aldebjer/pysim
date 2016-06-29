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

    void connect(char* outputname, CommonSystemImpl* inputsys, char* inputname);

protected:
    std::unique_ptr<CommonSystemImplPrivate> d_ptr;

};

