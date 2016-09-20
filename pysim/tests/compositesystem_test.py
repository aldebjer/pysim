"""Tests the compositesystem
"""
import numpy as np

from pysim.simulation import Sim
from pysim.systems import MassSpringDamper
from pysim.systems import SquareWave

from pysim.compositesystem import CompositeSystem

class ControlledSpring(CompositeSystem):
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

        self.add_input_port("force","msd","f", "force acting on the mass")
        self.add_output_port("position","msd","x1", "Position")

class CompositeSquareWave(CompositeSystem):
    """Composite system representating a square wave, used for testing."""
    def __init__(self):
        wave_sys = SquareWave()
        wave_sys.inputs.amplitude = 50
        wave_sys.inputs.freq = 0.1
        self.add_subsystem(wave_sys,"wave_sys")

        self.add_input_port("freq",
                            "wave_sys",
                            "freq",
                            "frequency of wave")
        self.add_input_port("amplitude",
                            "wave_sys",
                            "amplitude",
                            "amplitude of wave")
        self.add_output_port("signal",
                             "wave_sys",
                             "signal",
                             "signal from wave")

def test_connected_subsystems():
    """Test that subsystems can be connected"""

    cd = ControlledSpring()
    sim = Sim()
    sim.add_system(cd)

    sim.simulate(2, 0.1)
    assert np.abs(cd.outputs.out-0.3240587706226495) < 1e-10

def test_connection_from_composite():
    """Test that it is possible to connect from a composite system to an
    ordinary.
    """
    sim = Sim()

    msd = MassSpringDamper()
    msd.inputs.b = 80
    msd.inputs.m = 50
    msd.inputs.f = 0
    sim.add_system(msd)

    sw = CompositeSquareWave()
    sw.inputs.amplitude = 50
    sw.inputs.freq = 0.1
    sim.add_system(sw)

    sw.connections.add_connection("signal",msd,"f")
    sim.simulate(2, 0.1)
    assert np.abs(msd.states.x1 - 0.3240587706226495) < 1e-10
    
def test_connection_to_composite():
    """Test that it is possible to connect from an ordinary system to
    a composite
    """
    sim = Sim()

    msd = CompositeSpring()
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

    assert np.abs(cd.res.position[5]-0.90450499) < 1e-7
    assert np.abs(cd.res.position[-1]-0.32405877) < 1e-7



if __name__ == "__main__":
    test_connection_to_composite()
