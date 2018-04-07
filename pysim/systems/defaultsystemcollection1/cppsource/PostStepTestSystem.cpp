#include "PostStepTestSystem.h"
#include "factory.hpp"

void PostStepTestSystem::postStep() {
	state_scalar *= 2;
	state_vector *= 2;
	state_matrix *= 2;
}

REGISTER_SYSTEM(PostStepTestSystem);