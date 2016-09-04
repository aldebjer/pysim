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
        #msd.store("x1")
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

def test_connected_system():
    """Test of composite system - Work in Progress!"""

    cd = ControlledSpring()
    sim = Sim()
    sim.add_system(cd)

    sim.simulate(5, 0.1)

    assert np.abs(cd.outputs.out) < 0.1

if __name__ == "__main__":
    test_connected_system()
