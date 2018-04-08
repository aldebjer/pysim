#pragma once
#include "InOutTestSystem.h"

class PreStepTestSystem :
	public InOutTestSystem
{
public:
	PreStepTestSystem(void);
	void preStep();

protected:
	pysim::vector state_vector_derived;
};