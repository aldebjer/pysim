#pragma once
#include <vector>
#include <map>
#include <memory>

#include "StoreHandler.hpp"

#include "SimulatableSystem.hpp"

#include "Variable.hpp"

namespace pysim {

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

    Variable inputs;
    Variable outputs;
    Variable states;
    Variable ders;

    bool getDiscrete();

    void connect(char* outputname, CommonSystemImpl* inputsys, char* inputname);

protected:
    std::unique_ptr<CommonSystemImplPrivate> d_ptr;

};

}