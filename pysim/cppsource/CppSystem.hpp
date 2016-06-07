//   Copyright (c) 2014-2016 SSPA Sweden AB

#pragma once

#include "SimulatableSystem.hpp"
#include "PysimTypes.hpp"

#include <vector>
#include <map>
#include <utility>
#include <string>
#include <memory>

#include <boost/format.hpp>

class StoreHandler;

template <class T>
struct StateType {
    T stateValue;
    T derValue;
    std::string description;
};

template<typename T>
std::vector<std::string> getMapKeyStrings(const std::map<std::string, T>& map){
    std::vector<std::string> names;
    for (auto i = map.cbegin(); i != map.cend(); ++i){
        names.push_back(i->first);
    }
    return names;
}

template<typename T>
T getValue(std::string name, std::map<std::string, T*> map){
    if (map.count(name) < 1){

        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(map.at(name));
}

class CppSystem :
    public SimulatableSystem
{
public:
    CppSystem();
    virtual ~CppSystem();

    ////////////////////////////////////////
    //
    //        Inherited from System
    //
    ////////////////////////////////////////
    virtual void preSim(){};
    virtual void doStep(double time)=0;
    virtual void doStoreStep(double time);

    void copyoutputs(){
        for (auto vi = outputvector.cbegin(); vi != outputvector.cend(); ++vi){
            *(vi->second)=*(vi->first);
        }

        for (auto vi = connectedBoostvectors.cbegin(); vi != connectedBoostvectors.cend(); ++vi) {
            *(vi->second) = *(vi->first);
        }
    }

    void copystateoutputs(){
        for (auto vi = stateOutputvector.cbegin(); vi != stateOutputvector.cend(); ++vi){
            *(vi->second) = *(vi->first);
        }
    }

    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();

    double getNextUpdateTime(){ return nextUpdateTime;};
    bool getDiscrete() { return isDiscrete; };

    ///////////////////////////////////////
    //
    //      Python Interface
    //
    ///////////////////////////////////////

    //Store handling
    void store(char* name);
    const std::vector<double>& getStoreVector(char* name);
    void fillWithStore(char* name, double* p,int rows, int columns);
    void fillWithTime(double* p);
    int getStoreSize();
    int getStoreColumns(char* name);
    void setStoreInterval(double interval);
    std::vector<std::string> getStoreNames();

    //Input handling
    //get names
    std::vector<std::string> getInputNames(){ return getMapKeyStrings(inputs); };
    std::vector<std::string> getInputVectorNames();
    std::vector<std::string> getInputMatrixNames();
    std::vector<std::string> getInputStringNames() { return getMapKeyStrings(input_strings); };
    std::vector<std::string> getInputMapNames() { return getMapKeyStrings(input_maps); };

    //get values
    double getInput(char* name){ return getValue(name, inputs); };
    std::vector<double> getInputVector(char* name);
    std::vector<std::vector<double>> getInputMatrix(char* name);
    std::string getInputString(char* name) { return getValue(name, input_strings); };
    std::map<std::string, double> getInputMap(char* name);

    //set
    void setInput(char* name, double value);
    void setInputVector(char* name, std::vector<double> value);
    void setInputMatrix(char* name, std::vector<std::vector<double>> value);
    void setInputString(char* name, std::string value);
    void setInputMap(char* name, std::map<std::string, double> value);

    //descriptions
    std::map<std::string, std::string> getInputDescriptionMap(){ return input_descriptions; };

    //Output handling
    //names
    std::vector<std::string> getOutputNames(){ return getMapKeyStrings(outputs); };
    std::vector<std::string> getOutputVectorNames();

    //values
    double getOutput(char* name) { return getValue(name, outputs); };
    std::vector<double> getOutputVector(char* name);

    //descriptions
    std::map<std::string, std::string> getOutputDescriptionMap(){ return output_descriptions; };

    //State handling
    //get names
    std::vector<std::string> getStateNames(){ return getMapKeyStrings(statemap); };
    std::vector<std::string> getStateVectorNames();

    //get values
    double getState(char* name);
    std::vector<double> getStateVector(char* name);

    //set
    void setState(char* statename, double value);
    void setStateVector(char* statename, std::vector<double> value);

    //descriptions
    std::map<std::string, std::string> getStateDescriptionMap();

    void connect(char* outputname, CppSystem* inputsys, char* inputname);

    //Compare handling
    ////////////////////
    void add_compare_greater(char* comparename, double comparevalue);
    void add_compare_smaller(char* comparename, double comparevalue);
    bool do_comparison();

protected:

    ///////////////////////////////////////
    //
    //      Cpp Interface
    //
    ///////////////////////////////////////

    void state(double* state, const char* stateName, double* der, const char* derName, const char* desc = "");
    void state(pysim::vector* state, const char* stateName, pysim::vector* der, const char* derName, const char* desc = "");

    void input(double* var, const char* name, const char* desc);
    void input(std::vector<double>* vars, const char* name, const char* desc);
    void input(std::vector<std::vector<double>>* vars, const char* name, const char* desc);
    void input(pysim::vector* vars, const char* name, const char* description);
    void input(pysim::matrix* vars, const char* name, const char* description);
    void input(std::string*, const char* name, const char* description);
    void input(std::map<std::string, double>*, const char* name, const char* description);

    void output(double* var, const char* name, const char* desc);
    void output(std::vector<double>* vars, const char* name, const char* desc);
    void output(pysim::vector* vars, const char* name, const char* desc);

    void setNextUpdate(double t){ nextUpdateTime = t; };
    void setDiscrete(bool d = true) { isDiscrete = d; };



private:
    std::map<std::string, StateType<double*>> statemap;
    std::map<std::string, StateType<pysim::vector*> > state_boost_vectorsmap;

    std::map<std::string, StateType<double*>> dermap;
    std::map<std::string, StateType<pysim::vector*> > der_boost_vectorsmap;

    std::map<std::string, double*> inputs;
    std::map<std::string, std::vector<double>* > input_vectors;
    std::map<std::string, pysim::vector* > input_boost_vectors;
    std::map<std::string, std::vector<std::vector<double>>* > input_matrices;
    std::map<std::string, pysim::matrix* > input_boost_matrices;
    std::map<std::string, std::string*> input_strings;
    std::map<std::string, std::map<std::string, double>* > input_maps;
    std::map<std::string, std::string> input_descriptions;

    std::map<std::string, double*> outputs;
    std::map<std::string, std::vector<double>* > output_vectors;
    std::map<std::string, pysim::vector* > output_boost_vectors;
    std::map<std::string, std::string> output_descriptions;

    std::vector<std::pair<double*, double* > > outputvector;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connectedBoostvectors;
    std::vector<std::pair<double*, double* > > stateOutputvector;

    std::vector<std::pair<double*, double > > compare_greater_vector;
    std::vector<std::pair<double*, double > > compare_smaller_vector;

    double nextUpdateTime;
    bool isDiscrete;

    std::auto_ptr<StoreHandler> storeHandlerP;




};

#define STATE(x,dx,s) state(&x,#x,&dx,#dx,s);

#define INPUT(x,s) input(&x,#x,s);
#define OUTPUT(x,s) output(&x,#x,s);