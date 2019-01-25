"""Tests the compositesystem
"""
import numpy as np
from numpy.testing import assert_array_almost_equal
import pytest

from pysim.simulation import Sim
from pysim.systems import MassSpringDamper
from pysim.systems import SquareWave
from pysim.systems import InOutTestSystem
from pysim.systems import ControlledSpring as ControlledSpringCpp

from pysim.cythonsystem import Sys

class ControlledSpring(Sys):
    """Composite system created for testing
    The system is made up of a square wave system and a mass-spring-damper 
    system. The square wave is applied as a force acting on the mass,
    driving it up and down.
    """
    def __init__(self):
        msd = MassSpringDamper()
        msd.inputs.b = 80
        msd.inputs.m = 50
        msd.inputs.f = 0
        self.add_subsystem(msd,"msd")

        wave_sys = SquareWave()
        wave_sys.inputs.amplitude = 50
        wave_sys.inputs.freq = 0.1
        self.add_subsystem(wave_sys,"wave_sys")

        wave_sys.connections.add_connection("signal", msd, "f")

        self.add_input_scalar("amp")
        self.connections.add_connection("amp", wave_sys, "amplitude")

        self.add_output_scalar("out")
        msd.connections.add_connection("x1", self, "out")

        self.add_output_scalar("signal")
        wave_sys.connections.add_connection("signal", self, "signal")


class CompositeSpring(Sys):
    """Composite system for testing purposes
    The system contains only a mass spring damper subsystem connected to 
    ports.
    """
    def __init__(self):
        msd = MassSpringDamper()
        msd.inputs.b = 80
        msd.inputs.m = 50
        msd.inputs.f = 0
        self.add_subsystem(msd,"msd")

        self.add_input_scalar("force", "force acting on mass")
        self.inputs.force = 0
        self.connections.add_connection("force", msd, "f")

        self.add_output_scalar("position", "Position")
        msd.connections.add_connection("x1", self, "position")


class NestedCompositeSpring(Sys):
    """Nested composite system for testing purposes
    The system contains only a CompositeSpring system. It is used to 
    test nesting of composite systems. 
    """
    def __init__(self):
        composite_spring = CompositeSpring()
        self.add_subsystem(composite_spring,"composite_spring")

        self.add_input_scalar("force", "force acting on mass")
        self.inputs.force = 0
        self.connections.add_connection("force", composite_spring, "force")

        self.add_output_scalar("position", "Position of mass")
        self.outputs.position = 0
        composite_spring.connections.add_connection("position", self, "position")


class CompositeSquareWave(Sys):
    """Composite system representating a square wave, used for testing."""
    def __init__(self):
        wave_sys = SquareWave()
        self.add_subsystem(wave_sys,"wave_sys")

        self.add_input_scalar("freq", "frequency of wave")
        self.inputs.freq = 0.1
        self.connections.add_connection("freq", wave_sys, "freq")

        self.add_input_scalar("amplitude", "amplitude of wave")
        self.inputs.amplitude = 50
        self.connections.add_connection("amplitude", wave_sys, "amplitude")

        self.add_output_scalar("signal", "signal from wave")
        wave_sys.connections.add_connection("signal", self, "signal")


class NestedCompositeSquareWave(Sys):
    """Composite system representating a square wave, used for testing."""
    def __init__(self):
        wave_sys = CompositeSquareWave()
        self.add_subsystem(wave_sys,"wave_sys")

        self.add_input_scalar("freq", "frequency of wave")
        self.inputs.freq = 0.1
        self.connections.add_connection("freq", wave_sys, "freq")

        self.add_input_scalar("amplitude", "amplitude of wave")
        self.inputs.amplitude = 50
        self.connections.add_connection("amplitude", wave_sys, "amplitude")

        self.add_output_scalar("signal", "signal from wave")
        wave_sys.connections.add_connection("signal", self, "signal")

class CompositeTestSystem(Sys):
    """Composite system used for testing composite systems."""
    def __init__(self):
        self.new_subsystems = [InOutTestSystem(),
                               InOutTestSystem()]

        for i, sys in enumerate(self.new_subsystems):
            self.add_subsystem(sys,"subsystem_{}".format(i))

            name = "scalar_in_{}".format(i)
            self.add_input_scalar(name)
            setattr(self.inputs, name, -1)
            self.connections.add_connection(name, sys, "input_scalar")

            name = "output_scalar_{}".format(i)
            self.add_output_scalar(name)
            setattr(self.outputs, name, -1)
            sys.connections.add_connection("input_output_scalar", self, name)


            name = "vector_in_{}".format(i)
            self.add_input_vector(name, 3)
            setattr(self.inputs, name, [1,2,3])
            self.connections.add_connection(name, sys, "input_vector")

            name = "output_vector_{}".format(i)
            self.add_output_vector(name, 3)
            setattr(self.outputs, name, [-1,-1,-1])
            sys.connections.add_connection("input_output_vector", self, name)


            name = "output_state_vector_{}".format(i)
            self.add_output_vector(name, 3)
            setattr(self.outputs, name, [-1,-1,-1])
            sys.connections.add_connection("state_vector", self, name)


            name = "matrix_in_{}".format(i)
            self.add_input_matrix(name, 3, 3)
            self.connections.add_connection(name, sys, "input_matrix")

            name = "output_matrix_{}".format(i)
            self.add_output_matrix(name, 3, 3)
            setattr(self.outputs, name, ((-1,-1,-1),(-1,-1,-1),(-1,-1,-1)))
            sys.connections.add_connection("input_output_matrix", self, name)


            name = "output_state_matrix_{}".format(i)
            self.add_output_matrix(name, 3, 3)
            setattr(self.outputs, name, ((-1,-1,-1),(-1,-1,-1),(-1,-1,-1)))
            sys.connections.add_connection("state_matrix", self, name)


def test_port_connections():
    """Test the port connections to and from subsystems"""
    cs = CompositeTestSystem()
    ref_scalar = 5.0
    ref_vector = (6.0, 7.0, 8.0)
    ref_matrix = ((9.0, 10.0, 11.0),(12.0, 13.0, 14.0),(15.0, 16.0, 17.0))
    cs.inputs.scalar_in_0 = ref_scalar
    cs.inputs.vector_in_0 = ref_vector
    cs.inputs.matrix_in_0 = ref_matrix
    sim = Sim()
    sim.add_system(cs)
    sim.simulate(0.5, 0.1)

    assert cs.outputs.output_scalar_0 == ref_scalar
    assert_array_almost_equal(cs.outputs.output_vector_0, ref_vector, 18)
    assert_array_almost_equal(cs.outputs.output_matrix_0,ref_matrix, 18)

def test_connected_subsystems():
    """Test that subsystems can be connected"""

    cd = ControlledSpring()
    sim = Sim()
    sim.add_system(cd)

    sim.simulate(2, 0.1)
    print(cd.outputs.out)
    assert np.abs(cd.outputs.out-0.3240587706226495) < 1e-10

@pytest.mark.parametrize("sw_class",
                         [CompositeSquareWave,NestedCompositeSquareWave])
def test_connection_from_composite(sw_class):
    """Test that it is possible to connect from a composite system to an
    ordinary.
    """
    sim = Sim()

    msd = MassSpringDamper()
    msd.inputs.b = 80
    msd.inputs.m = 50
    msd.inputs.f = 0
    sim.add_system(msd)

    sw = sw_class()
    sw.inputs.amplitude = 50
    sw.inputs.freq = 0.1
    sim.add_system(sw)

    sw.connections.add_connection("signal",msd,"f")
    sim.simulate(2, 0.1)
    assert np.abs(msd.states.x1 - 0.3240587706226495) < 1e-10

@pytest.mark.parametrize("spring_class",[CompositeSpring,NestedCompositeSpring])
def test_connection_to_composite(spring_class):
    """Test that it is possible to connect from an ordinary system to
    a composite
    """
    sim = Sim()

    msd = spring_class()
    sim.add_system(msd)

    sw = SquareWave()
    sw.inputs.amplitude = 50
    sw.inputs.freq = 0.1
    sim.add_system(sw)

    sw.connections.add_connection("signal",msd,"force")
    sim.simulate(2, 0.1)
    assert np.abs(msd.outputs.position - 0.3240587706226495) < 1e-10


def test_system_store():
    """Test that it is possible to store the output from a composite system"""
    cd = CompositeSpring()
    sim = Sim()
    sim.add_system(cd)
    cd.store("position")

    sim.simulate(2, 0.1)

    assert np.abs(cd.res.position[5]-0.90450499444532406) < 1e-7
    assert np.abs(cd.res.position[-1]-0.3240587706226495) < 1e-7

def test_composite_vs_connected_outputs():
    """Test that the same result is given regardless if two systems are 
    connected externally or put together in a composite system"""

    sim = Sim()

    #Externally connected systems
    msd = MassSpringDamper()
    msd.inputs.b = 80
    msd.inputs.m = 50
    msd.inputs.f = 0
    sim.add_system(msd)

    sw = SquareWave()
    sw.inputs.amplitude = 50
    sw.inputs.freq = 0.1
    sim.add_system(sw)

    sw.connections.add_connection("signal",msd,"f")

    #Composite system
    cd = ControlledSpring()
    cd.inputs.amp = 50
    sim.add_system(cd)

    cd2 = ControlledSpringCpp()
    sim.add_system(cd2)

    sim.simulate(2, 0.1)

    assert cd.outputs.out == msd.states.x1 == cd2.outputs.out



if __name__ == "__main__":
    #test_connected_subsystems()
    #test_connection_from_composite(CompositeSquareWave)
    #test_connection_to_composite(NestedCompositeSpring)
    #test_port_connections()
    #test_system_store()
    test_composite_vs_connected_outputs()
