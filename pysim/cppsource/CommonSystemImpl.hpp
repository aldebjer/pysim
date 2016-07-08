#pragma once
#include <vector>
#include <map>
#include <memory>

#include "StoreHandler.hpp"

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

    double getNextUpdateTime();
    bool do_comparison();

    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();

    void doStoreStep(double time);
    void store(char* name);
    StoreHandler* getStoreHandlerP();


    void add_compare_greater(char* comparename, double comparevalue);
    void add_compare_smaller(char* comparename, double comparevalue);

    //Parameter handling
    std::vector<std::string> getParStringNames();
    std::vector<std::string> getParMatrixNames();
    std::vector<std::string> getParMapNames();
    std::string getParString(char* name);
    void setParString(char* name, std::string value);
    std::vector<std::vector<double>> getParMatrix(char* name);
    void setParMatrix(char* name, std::vector<std::vector<double>> value);
    std::map<std::string, double> getParMap(char* name);
    void setParMap(char* name, std::map<std::string, double> value);

    std::map<std::string, std::string> getParDescriptionMap();

    //Input handling
    std::vector<std::string> getScalarInputNames();
    std::vector<std::string> getInputVectorNames();
    void setScalarInput(char* name, double value);
    void setInputVector(char* name, std::vector<double> value);
    double getScalarInput(char* name);
    std::vector<double> getInputVector(char* name);
    std::map<std::string, std::string> getInputDescriptionMap();

    //Output handling
    std::vector<std::string> getScalarOutputNames();
    std::vector<std::string> getOutputVectorNames();
    double getScalarOutput(char* name);
    std::vector<double> getOutputVector(char* name);
    void setScalarOutput(char* name, double value);
    void setOutputVector(char* name, std::vector<double> value);
    std::map<std::string, std::string> getOutputDescriptionMap();

    //State handling
    std::vector<std::string> getScalarStateNames();
    std::vector<std::string> getStateVectorNames();
    double getScalarState(char* name);
    std::vector<double> getStateVector(char* name);
    void setStateVector(char* name, std::vector<double> value);
    void setScalarState(char* name, double value);

    std::map<std::string, std::string> getStateDescriptionMap();

    //Der handling
    std::vector<std::string> getScalarDerNames();
    std::vector<std::string> getDerVectorNames();
    double getScalarDer(char* name);
    std::vector<double> getDerVector(char* name);
    void setDerVector(char* name, std::vector<double> value);
    void setScalarDer(char* name, double value);
    std::map<std::string, std::string> getDerDescriptionMap();

    bool getDiscrete();

    void connect(char* outputname, CommonSystemImpl* inputsys, char* inputname);

protected:
    std::unique_ptr<CommonSystemImplPrivate> d_ptr;

};

