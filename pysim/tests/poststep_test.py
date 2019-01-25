""" Test poststep functionality """

import numpy as np
from numpy.testing import assert_array_almost_equal
import pytest

from pysim.simulation import Sim
from pysim.systems.python_systems import InOutTestSystem
from pysim.systems import PostStepTestSystem as PostStepTestSystemCpp

from pysim.cythonsystem import Sys


class PostStepTestSystem(InOutTestSystem):
    """Test system for testing post step functionality 
    for a cython system"""
    def __init__(self):
        super().__init__()

    def post_step(self):
        self.states.state_scalar = 1.23*2
        self.states.state_vector = np.ones(3)*4.56*2
        self.states.state_matrix = np.ones((3,3))*7.89*2

class PostStepCompositeSystem(Sys):
    """Composite test system for testing post step 
    functionality within a composite system"""
    def __init__(self):
        ps = PostStepTestSystem()
        self.add_subsystem(ps, "ps")

        self.add_output_scalar("state_scalar_out")
        self.outputs.state_scalar_out = 0
        ps.connections.add_connection("state_scalar", self, "state_scalar_out")

        self.add_output_vector("state_vector_out", 3)
        self.outputs.state_vector_out = [0,0,0]
        ps.connections.add_connection("state_vector", self, "state_vector_out")

        self.add_output_matrix("state_matrix_out", 3, 3)
        self.outputs.state_matrix_out = [[0,0,0],[0,0,0],[0,0,0]]
        ps.connections.add_connection("state_matrix", self, "state_matrix_out")


def test_cython_poststep():
    """Test that cython post step functionality is working"""
    sim = Sim()

    ps = PostStepTestSystem()

    sim.add_system(ps)
    sim.simulate(2, 0.1)

    assert ps.states.state_scalar == 1.23*2
    assert np.all(ps.states.state_vector == np.ones(3)*4.56*2)
    assert np.all(ps.states.state_matrix == np.ones((3,3))*7.89*2)

def test_composite_poststep():
    """Test that composite post step functionality is working"""
    sim = Sim()
    ps = PostStepCompositeSystem()

    sim.add_system(ps)
    sim.simulate(2, 0.1)

    assert ps.outputs.state_scalar_out == 1.23*2
    assert np.all(ps.outputs.state_vector_out == np.ones(3)*4.56*2)
    assert np.all(ps.outputs.state_matrix_out == np.ones((3,3))*7.89*2)

def test_cpp_poststep():
    """Test that cpp system post step functionality is working"""

    sim = Sim()

    ps = PostStepTestSystemCpp()
    sim.add_system(ps)
    sim.simulate(2, 0.1)

    assert ps.states.state_scalar == 1.23*2
    assert np.all(ps.states.state_vector == np.ones(3)*4.56*2)
    assert np.all(ps.states.state_matrix == np.zeros((3,3)))


if __name__ == '__main__':
    #test_cython_poststep()
    test_composite_poststep()
    #test_cpp_poststep()