"""This module contains tests for for the parameter functionality
"""

from pysim.simulation import Sim
from pysim.systems import ParameterTestSystem

def get_system():
    """Get a default parameter test system and set it up"""
    sys = ParameterTestSystem()
    sys.pars.parameter_vector = [1,2,3]
    sys.pars.parameter_matrix = [[1,2,3],[4,5,6]]
    sys.pars.parameter_map = {'a':1, 'b':2}
    sys.pars.parameter_vectormap = {'a':[1,2], 'b':[3,4]}
    return sys

def test_vector_parameter():
    """Test that it is possible to set a vector parameter"""
    sys = get_system()
    assert sys.pars.parameter_vector == [1,2,3]
    sim = Sim()
    sim.add_system(sys)
    sim.simulate(0.1,0.1)
    assert sys.outputs.output_from_vector == 1

def test_matrix_parameter():
    """Test that it is possible to set a vector parameter"""
    sys = get_system()
    assert sys.pars.parameter_matrix == [[1,2,3],[4,5,6]]
    sim = Sim()
    sim.add_system(sys)
    sim.simulate(0.1,0.1)
    assert sys.outputs.output_from_matrix == 1

def test_map_parameter():
    """Test that it is possible to set a map parameter"""
    sys = get_system()
    assert sys.pars.parameter_map == {'a':1, 'b':2}
    sim = Sim()
    sim.add_system(sys)
    sim.simulate(0.1,0.1)
    assert sys.outputs.output_from_map == 1

def test_vectormap_parameter():
    """Test that it is possible to set a vectormap parameter"""
    sys = get_system()
    assert sys.pars.parameter_vectormap == {'a':[1,2], 'b':[3,4]}
    sim = Sim()
    sim.add_system(sys)
    sim.simulate(0.1,0.1)
    assert sys.outputs.output_from_vectormap == 2

if __name__ == "__main__":
    test_map_parameter()
