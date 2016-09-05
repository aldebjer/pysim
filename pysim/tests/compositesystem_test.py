"""Tests the compositesystem
"""
import numpy as np
import pytest

from pysim.simulation import Sim
from pysim.systems import MassSpringDamper
from pysim.systems import SquareWave

from pysim.compositesystem import CompositeSystem

class ControlledSpring(CompositeSystem):
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
    def __init__(self):
        msd = MassSpringDamper()
        msd.inputs.b = 80
        msd.inputs.m = 50
        msd.inputs.f = 0
        self.add_subsystem(msd,"msd")

        self.add_input_port("force","msd","f", "force acting on the mass")
        self.add_output_port("position","msd","x1", "Position")

def test_connected_subsystems():
    """Test that subsystems can be connected"""

    cd = ControlledSpring()
    sim = Sim()
    sim.add_system(cd)

    sim.simulate(2, 0.1)
    assert np.abs(cd.outputs.out-0.3240587706226495) < 1e-10

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
    test_connected_subsystems()
