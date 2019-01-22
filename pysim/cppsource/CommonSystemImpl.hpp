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
	//      C++ interface
	//////////////////////////////////////////////////////////////////////////
	virtual void preSim() {};
	virtual void preStep() {};
	virtual void doStep(double time) = 0;
	virtual void postStep() {};
	virtual void copyinputs() {};
	virtual void copyoutputs() {};
	virtual void copystateoutputs() {};

    //////////////////////////////////////////////////////////////////////////
    //      Inherited from Simulatable System
    //////////////////////////////////////////////////////////////////////////
	void __preSim();
	void __preStep();
	void __doStep(double time);
	void __postStep();
	void __copyinputs();
	void __copyoutputs();
    void __copystateoutputs();
    double getNextUpdateTime();
    bool do_comparison();
    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();
    void doStoreStep(double time);
    bool getDiscrete();


    //////////////////////////////////////////////////////////////////////////
    //      Interface used from python
    //////////////////////////////////////////////////////////////////////////
    void store(const char* name);
    StoreHandler* getStoreHandlerP();

	void add_subsystem(CommonSystemImpl * subsystem, std::string name);

    void add_compare_greater(char* comparename, double comparevalue);
    void add_compare_smaller(char* comparename, double comparevalue);

    //Parameter handling
    template <typename T> std::vector<std::string> getParNames();
    template <typename T> T getPar(char* name);
    template <typename T> void setPar(char* name, T value);

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