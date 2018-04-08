""" Test prestep functionality """

import numpy as np
from numpy.testing import assert_array_almost_equal
import pytest

from pysim.simulation import Sim

from pysim.compositesystem import CompositeSystem
from pysim.systems.python_systems import InOutTestSystem
from pysim.systems import PreStepTestSystem as PreStepTestSystemCpp

class PreStepTestSystem(InOutTestSystem):
    """Test system for testing post step functionality 
    for a cython system"""
    def __init__(self):
        super().__init__()
        self.add_output_vector('state_vector_derived', 3, '')
        self.system_pos = np.array([10, 5, -2])

    def pre_step(self):
        self.outputs.state_vector_derived = np.cross(self.states.state_vector, self.system_pos)


class PreStepCompositeSystem(CompositeSystem):
    """Composite test system for testing post step 
    functionality within a composite system"""
    def __init__(self):
        ps = PreStepTestSystem()
        self.add_subsystem(ps, 'ps')

        self.expand_single_output('state_vector_derived',
                                  ps,
                                  'state_vector_derived',
                                  [0,0,0])



def test_cython_prestep():
    """Test that cython post step functionality is working"""
    sim = Sim()

    ps = PreStepTestSystem()

    sim.add_system(ps)
    sim.simulate(1, 1)

    assert np.all(ps.outputs.state_vector_derived == 
                  [-31.919999999999995, 54.71999999999999, -22.799999999999997])

def test_composite_prestep():
    """Test that composite post step functionality is working"""
    sim = Sim()
    ps = PreStepCompositeSystem()

    sim.add_system(ps)
    sim.simulate(1, 1)

    assert np.all(ps.outputs.state_vector_derived == 
                  [-31.919999999999995, 54.71999999999999, -22.799999999999997])

def test_cpp_prestep():
    """Test that cpp system post step functionality is working"""

    sim = Sim()

    ps = PreStepTestSystemCpp()
    sim.add_system(ps)
    sim.simulate(1, 1)

    np.all(ps.outputs.state_vector_derived ==
                  [-31.919999999999995, 54.71999999999999, -22.799999999999997])


if __name__ == '__main__':
    test_cython_prestep()
    test_composite_prestep()
    test_cpp_prestep()