#pragma once
#include "CppSystem.hpp"

class RigidBody :
    public pysim::CppSystem
{
public:
    RigidBody(void);

    static std::string getDocs();

    //First calculation in a simulation, only done once
    void preSim();

    //This function is called for each evaluation
    void doStep(double time);

    //After each set of evaluations when a new state value 
    //has been calculated this section is called
    void postStep(){};

    //Before finishing the simulation this function is called
    void finally(){};

protected:

    //Inputs
    pysim::vector force;
    pysim::vector moment;

    double mass;
    pysim::matrix inertia;
    pysim::vector cog;

    //States
    pysim::vector position, d_pos;
    pysim::vector velocity, d_velocity;
    pysim::vector attitude, d_att;
    pysim::vector rotation_vel, d_rotation_vel;

    //Outputs
    pysim::vector acceleration;

    //Internal
    pysim::vector to_global(pysim::vector);
};