#pragma once
#include <vector>
#include <map>
#include <memory>


#include "SimulatableSystem.hpp"

#include "Variable.hpp"

#include "ConnectionHandler.hpp"

namespace pysim {

struct CommonSystemImplPrivate;
class StoreHandler;

class  CommonSystemImpl :
    public SimulatableSystemInterface
{
public:
    CommonSystemImpl();
    virtual ~CommonSystemImpl();

    //////////////////////////////////////////////////////////////////////////
    //      Inherited from Simulatable System
    //////////////////////////////////////////////////////////////////////////
    void copyoutputs();
    void copystateoutputs();
    double getNextUpdateTime();
    bool do_comparison();
    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();
    void doStoreStep(double time);
    bool getDiscrete();


    //////////////////////////////////////////////////////////////////////////
    //      Interface used from python
    //////////////////////////////////////////////////////////////////////////
    void store(char* name);
    StoreHandler* getStoreHandlerP();

    void add_compare_greater(char* comparename, double comparevalue);
    void add_compare_smaller(char* comparename, double comparevalue);

    //Parameter handling
    std::vector<std::string> getParStringNames();
    std::vector<std::string> getParMatrixNames();
    std::vector<std::string> getParVectorNames();
    std::vector<std::string> getParMapNames();
    std::string getParString(char* name);
    void setParString(char* name, std::string value);
    std::vector<double> getParVector(char* name);
    void setParVector(char* name, std::vector<double> value);
    std::vector<std::vector<double>> getParMatrix(char* name);
    void setParMatrix(char* name, std::vector<std::vector<double>> value);
    std::map<std::string, double> getParMap(char* name);
    void setParMap(char* name, std::map<std::string, double> value);

    std::map<std::string, std::string> getParDescriptionMap();
    
    //////////////////////////////////////////////////////////////////////////
    //      Attributes
    //////////////////////////////////////////////////////////////////////////
    Variable inputs;
    Variable outputs;
    Variable states;
    Variable ders;
    ConnectionHandler connectionHandler;

protected:
    std::unique_ptr<CommonSystemImplPrivate> d_ptr;

};

}