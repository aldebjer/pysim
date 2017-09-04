#pragma once
#include "CppSystem.hpp"

class Adder6D :
	public pysim::CppSystem
{
public:
	Adder6D(void);

	static std::string getDocs();

	//First calculation in a simulation, only done once
	void preSim() {};

	//This function is called for each evaluation
	void doStep(double time);

	//After each set of evaluations when a new state value 
	//has been calculated this section is called
	void postStep() {};

	//Before finishing the simulation this function is called
	void finally() {};

protected:

	//Parameters
	std::vector<pysim::vector> inputs;

	//Variables
	pysim::vector out;

	static const int MAX_INPUTS = 25;
};