#include "RigidBody.h"

#include "factory.hpp"

using std::string;

string RigidBody::getDocs(){
    return string(
"System representing a rigid body in 3D\n\n"
);
}

RigidBody::RigidBody(void) :
    force(3, 0.0),
    moment(3, 0.0),
    mass(1.0),
    inertia(3, 3, 0.0),
    cog(3,0.0),
    position(3, 0.0), d_pos(3, 0.0),
    velocity(3, 0.0), d_velocity(3, 0.0),
    attitude(3, 0.0), d_att(3, 0.0),
    rotation_vel(3, 0.0), d_rotation_vel(3, 0.0)

{
    INPUT(force, "Force applied to the body, in body coordinates");
    INPUT(moment, "Moment applied to the body, in body coordinates");
    INPUT(mass, "The total mass of the body");
    //INPUT(inertia, "Inertia of the rigid body");
    INPUT(cog, "Center of gravity of the body");

    STATE(position, d_pos, "Position of the body in global coordinates");
    STATE(velocity, d_velocity, "Velocity of the body in local coordinates");
    STATE(attitude, d_att, "Attitude of the body");
    STATE(rotation_vel, d_rotation_vel, "Rotational velocity of the body");

    //OUTPUT(acceleration, "Acceleration of the body [m/s^2]");
}

//First calculation in a simulation, only done once
void RigidBody::preSim() {
};

pysim::vector cross_product(const pysim::vector& a, const pysim::vector& b) {
    pysim::vector out(3);
    out(0) = a(1)*b(2) - a(2)*b(1);
    out(1) = a(2)*b(0) - a(0)*b(2);
    out(2) = a(0)*b(1) - a(1)*b(0);
    return out;
}

pysim::matrix rotmat_toglob(double phi, double theta, double psi) {
    pysim::matrix out(3, 3);
    out(0, 0) = cos(psi)*cos(theta);
    out(0, 1) = cos(psi)*sin(theta)*sin(phi) - 1 * sin(psi)*cos(phi);
    out(0, 2) = sin(psi)*sin(phi) + cos(psi)*cos(phi)*sin(theta);
    out(1, 0) = sin(psi)*cos(theta);
    out(1, 1) = cos(psi)*cos(phi) + sin(phi)*sin(theta)*sin(phi);
    out(1, 2) = sin(theta)*sin(psi)*cos(phi);
    out(2, 0) = -1 * sin(theta);
    out(2, 1) = cos(theta)*sin(phi);
    out(2, 2) = cos(theta)*cos(phi);
    return out;
}

pysim::matrix rotmat_toglob(const pysim::vector& omega) {
    double phi = omega(0);
    double theta = omega(1);
    double psi = omega(2);
    return rotmat_toglob(phi, theta, psi);
}

pysim::vector RigidBody::to_global(pysim::vector v) {
    pysim::matrix R = rotmat_toglob(attitude);
    return prod(R, v);
}


void RigidBody::doStep(double time){
    pysim::vector euler = cross_product(d_rotation_vel, cog);
    pysim::vector coriolis = cross_product(rotation_vel, velocity);
    pysim::vector centrifugal = cross_product(rotation_vel, cross_product(rotation_vel, cog));
    pysim::vector acc = force/mass;
    d_velocity = acc;// -euler - coriolis - centrifugal;
    d_pos = to_global(velocity);
}

REGISTER_SYSTEM(RigidBody);

