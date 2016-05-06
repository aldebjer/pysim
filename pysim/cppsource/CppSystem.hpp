//   Copyright (c) 2014-2016 SSPA Sweden AB

#pragma once

#include "SimulatableSystem.hpp"

#include <vector>
#include <map>
#include <utility>
#include <string>
#include <memory>

#include <boost/format.hpp>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
namespace pysim{
    typedef boost::numeric::ublas::vector<double> vector;
    typedef boost::numeric::ublas::matrix<double> matrix;
};

template <class T>
class StoreStruct {
public:
    StoreStruct(T* p)
        : valueP(p) {
    }

    T* valueP;
    std::vector<T> storearray;
};

class PointerVectorIter
{
    public:
    PointerVectorIter(std::vector<double*>* vec):
    p(vec),
    i(p->begin()){
    };

    double* getFirst(){
        i = p->begin();
        return getNext();
    };


    double* getNext(){
        if(i==p->end() ){
            return nullptr;
        }
        return *(i++);
    }

    protected:
    std::vector<double*>* p;
    std::vector<double*>::iterator i;
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

    double* getFirstState(){return stateIter.getFirst();};
    double* getNextState(){return stateIter.getNext();};
    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();
    double* getFirstDer(){return dersIter.getFirst();};
    double* getNextDer(){return dersIter.getNext();};

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
    double getState(char* name){ return getValue(name, statemap); };
    std::vector<double> getStateVector(char* name);
    std::vector<double*> getStateVector() { return states; };

    //set
    void setState(char* statename, double value);
    void setStateVector(char* statename, std::vector<double> value);

    //descriptions
    std::map<std::string, std::string> getStateDescriptionMap(){ return state_descriptions; };

    void connect(char* outputname, CppSystem* inputsys, char* inputname);




    std::vector<double*> getDerVector(){ return ders; };

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

    void state(double* state, const char* stateName, const char* desc = "");
    void state(std::vector<double>* state, const char* stateName, const char* desc = "");
    void state(pysim::vector* state, const char* stateName, const char* desc = "");
    void der(double* der, const char* derName);
    void der(std::vector<double>* der, const char* derName);
    void der(pysim::vector* der, const char* derName);

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

    std::vector<double*> states;
    std::vector<std::vector<double>*> statevectors;
    std::vector<pysim::vector*> state_boost_vectors;
    PointerVectorIter stateIter;
    std::map<std::string,double*> statemap;
    std::map<std::string, std::vector<double>* > statevectorsmap;
    std::map<std::string, pysim::vector* > state_boost_vectorsmap;
    std::map<std::string, std::string> state_descriptions;

    std::vector<double*> ders;
    std::vector<std::vector<double>*> dervectors;
    std::vector<pysim::vector*> der_boost_vectors;
    std::map<std::string,double*> dermap;
    std::map<std::string, std::vector<double>* > dervectorsmap;
    std::map<std::string, pysim::vector* > der_boost_vectorsmap;
    PointerVectorIter dersIter;

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

    std::map<std::string,std::shared_ptr<StoreStruct<double>>> storemap;
    std::map<std::string, std::shared_ptr<StoreStruct<pysim::vector>>> storeVectorMap;
    std::vector<double> storetimes;

    std::vector<std::pair<double*, double* > > outputvector;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connectedBoostvectors;
    std::vector<std::pair<double*, double* > > stateOutputvector;

    std::vector<std::pair<double*, double > > compare_greater_vector;
    std::vector<std::pair<double*, double > > compare_smaller_vector;

    double nextUpdateTime;
    bool isDiscrete;

    double storeInterval;
    double nextStoreTime;





};

#define STATE(x,dx,s) state(&x,#x,s);der(&dx,#dx);

#define INPUT(x,s) input(&x,#x,s);
#define OUTPUT(x,s) output(&x,#x,s);