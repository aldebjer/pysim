﻿"""Tests the compositesystem
"""
import numpy as np
from numpy.testing import assert_array_almost_equal
import pytest

from pysim.simulation import Sim
from pysim.systems import MassSpringDamper
from pysim.systems import SquareWave
from pysim.systems import InOutTestSystem

from pysim.compositesystem import CompositeSystem

class ControlledSpring(CompositeSystem):
    """Composite system created for testing
    The system is made up of a square wave system and a mass-spring-damper 
    system. The square wave is applied as a force acting on the mass,
    driving it up and down.
    """
    def __init__(self):
        wave_sys = SquareWave()
        wave_sys.inputs.amplitude = 50
        wave_sys.inputs.freq = 0.1
        self.add_subsystem(wave_sys,"wave_sys")

        msd = MassSpringDamper()
        msd.inputs.b = 80
        msd.inputs.m = 50
        msd.inputs.f = 0
        self.add_subsystem(msd,"msd")

        wave_sys.connections.add_connection("signal", msd, "f")

        self.add_input_port("amp","wave_sys","amplitude", "amplitude of wave")
        self.add_output_port("out","msd","x1", "position")
        self.add_output_port("signal","wave_sys","signal", "signal from wave")

class CompositeSpring(CompositeSystem):
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
        self.add_port_in_scalar("force", 0, "force acting on mass")
        self.connect_port_in("force", msd, "f")
        self.add_output_port("position","msd","x1", "Position")

class NestedCompositeSpring(CompositeSystem):
    """Nested composite system for testing purposes
    The system contains only a CompositeSpring system. It is used to 
    test nesting of composite systems. 
    """
    def __init__(self):
        composite_spring = CompositeSpring()
        self.add_subsystem(composite_spring,"composite_spring")
        self.add_port_in_scalar("force", 0, "force acting on mass")
        self.connect_port_in("force", composite_spring, "force")
        self.add_port_out_scalar("position",0,"Position of the mass")
        self.connect_port_out("position",composite_spring,"position")

class CompositeSquareWave(CompositeSystem):
    """Composite system representating a square wave, used for testing."""
    def __init__(self):
        wave_sys = SquareWave()
        wave_sys.inputs.amplitude = 50
        wave_sys.inputs.freq = 0.1
        self.add_subsystem(wave_sys,"wave_sys")

        self.add_port_in_scalar("freq", 0, "frequency of wave")
        self.connect_port_in("freq", wave_sys, "freq")

        self.add_port_in_scalar("amplitude", 0, "amplitude of wave")
        self.connect_port_in("amplitude", wave_sys, "amplitude")

        self.add_port_out_scalar("signal", 0, "signal from wave")
        self.connect_port_out("signal", wave_sys, "signal")

class NestedCompositeSquareWave(CompositeSystem):
    """Composite system representating a square wave, used for testing."""
    def __init__(self):
        wave_sys = CompositeSquareWave()
        wave_sys.inputs.amplitude = 50
        wave_sys.inputs.freq = 0.1
        self.add_subsystem(wave_sys,"wave_sys")

        self.add_port_in_scalar("freq", 0, "frequency of wave")
        self.connect_port_in("freq", wave_sys, "freq")

        self.add_port_in_scalar("amplitude", 0, "amplitude of wave")
        self.connect_port_in("amplitude", wave_sys, "amplitude")

        self.add_port_out_scalar("signal", 0, "signal from wave")
        self.connect_port_out("signal", wave_sys, "signal")

class CompositeTestSystem(CompositeSystem):
    """Composite system used for testing composite systems."""
    def __init__(self):
        self.new_subsystems = [InOutTestSystem(),
                           InOutTestSystem()]

        for i in range(len(self.new_subsystems)):
            self.add_subsystem(self.new_subsystems[i],"subsystem_{}".format(i))

            self.add_port_in_scalar("scalar_in_{}".format(i), -1, "input scalar {}".format(i))
            self.connect_port_in("scalar_in_{}".format(i), self.new_subsystems[i], "input_scalar")
            portoutname = "output_scalar_{}".format(i)
            self.add_port_out_scalar(portoutname,-1,"Test scalar output from composite system {}".format(i))
            self.connect_port_out(portoutname, self.new_subsystems[i], "input_output_scalar")

            self.add_port_in_vector("vector_in_{}".format(i), (1,2,3), "input vector {}".format(i))
            self.connect_port_in("vector_in_{}".format(i), self.new_subsystems[i], "input_vector")
            portoutname = "output_vector_{}".format(i)
            self.add_port_out_vector(portoutname,(-1, -1, -1),"Test vector output from composite system {}".format(i))
            self.connect_port_out(portoutname, self.new_subsystems[i], "input_output_vector")

            self.add_port_in_matrix("matrix_in_{}".format(i), ((1,2),(3,4)), "input matrix {}".format(i))
            self.connect_port_in("matrix_in_{}".format(i), self.new_subsystems[i], "input_matrix")
            portoutname = "output_matrix_{}".format(i)
            self.add_port_out_matrix(portoutname,((-1,-1),(-1,-1)),"Test matrix output from composite system {}".format(i))
            self.connect_port_out(portoutname, self.new_subsystems[i], "input_output_matrix")

def test_port_connections():
    """Test the port connections to and from subsystems"""
    cs = CompositeTestSystem()
    ref_scalar = 5.0
    ref_vector = (6.0, 7.0, 8.0)
    ref_matrix = ((9.0, 10.0),(11.0, 12.0))
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
    assert np.abs(cd.outputs.out-0.32406429942202225) < 1e-10

@pytest.mark.parametrize("sw_class",[CompositeSquareWave,NestedCompositeSquareWave])
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
    assert np.abs(msd.outputs.position - 0.32406429942202225) < 1e-10


def test_system_store():
    """Test that it is possible to store the output from a composite system"""
    cd = CompositeSpring()
    sim = Sim()
    sim.add_system(cd)
    cd.store("position")

    sim.simulate(2, 0.1)

    assert np.abs(cd.res.position[5]-0.90459733332768599) < 1e-7
    assert np.abs(cd.res.position[-1]-0.32406429942202225) < 1e-7



if __name__ == "__main__":
    #test_connected_subsystems()
    test_connection_from_composite()
    #test_connection_to_composite()
