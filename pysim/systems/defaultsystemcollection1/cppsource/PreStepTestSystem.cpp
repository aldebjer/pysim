#include "PreStepTestSystem.h"
#include "factory.hpp"

PreStepTestSystem::PreStepTestSystem(void):
	InOutTestSystem::InOutTestSystem(),
	state_vector_derived(3, 0.0) 
{
	OUTPUT(state_vector_derived, "");

}

void PreStepTestSystem::preStep() {
	pysim::vector system_pos(3);
	system_pos[0] = 10; system_pos[1] = 5; system_pos[2] = -2;

	state_vector_derived[0] = state_vector[1] * system_pos[2] -
		state_vector[2] * system_pos[1];
	state_vector_derived[1] = state_vector[2] * system_pos[0] -
		state_vector[0] * system_pos[2];
	state_vector_derived[2] = state_vector[0] * system_pos[1] -
		state_vector[1] * system_pos[0];
}

REGISTER_SYSTEM(PreStepTestSystem);