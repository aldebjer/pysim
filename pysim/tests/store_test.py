"""This module contains tests for the store function of pysim
"""
import numpy as np

import pytest

from pysim.simulation import Sim
from pysim.systems import VanDerPol, SquareWave, RigidBody, InOutTestSystem
from pysim.systems.python_systems import VanDerPol as PythonVanDerPol
from pysim.systems.python_systems import InOutTestSystem as PythonInOutTestSystem
from pysim.compositesystem import CompositeSystem

class CompositeVanDerPol(CompositeSystem):

    def __init__(self):
        vdp = VanDerPol()
        self.add_subsystem(vdp, 'vdp')

        self.expand_single_output('x', vdp, 'x', 0)
        self.expand_single_output('dx', vdp, 'dx', 0)
        self.expand_single_input('a', vdp, 'a', 0)

__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_missing_storename(test_class):
    """Test that an exception is raised when trying to store a non-existing
    variable."""
    sys = test_class()
    with pytest.raises(ValueError):
        sys.store("xyxyxy")

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_store_state(test_class):
    """Test that it is possible to store a state"""
    sim = Sim()
    sys = test_class()
    sys.store("x")

    sim.add_system(sys)
    sim.simulate(10, 0.1)
    xres = sys.res.x
    assert isinstance(xres, np.ndarray)
    assert xres.size == 101

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_store_der(test_class):
    """Test that it is possible to store a der"""
    sim = Sim()
    sys = test_class()
    sys.store("dx")

    sim.add_system(sys)
    sim.simulate(10, 0.1)
    dxres = sys.res.dx
    assert isinstance(dxres, np.ndarray)
    assert dxres.size == 101

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_store_input(test_class):
    """Test that it is possible to store an input"""
    sim = Sim()
    sys = test_class()
    sys.store("a")

    sim.add_system(sys)
    sim.simulate(10, 0.1)
    ares = sys.res.a
    assert isinstance(ares,np.ndarray)
    assert ares.size == 101
    assert np.all(ares==1.0)

def test_store_output():
    """Test that it is possible to store a state"""
    sim = Sim()
    sys = SquareWave()
    sys.store("signal")

    sim.add_system(sys)
    sim.simulate(10, 0.1)
    res = sys.res.signal
    assert isinstance(res,np.ndarray)
    assert res.size == 101

def test_store_vector():
    """Test that it is possible to store a boost vector"""
    sim = Sim()
    sys = RigidBody()
    sys.store("position")
    sys.inputs.force =  [1,0,0]
    sim.add_system(sys)
    sim.simulate(20,0.01)
    diff = sys.res.position[-1,:]-sys.states.position
    assert np.all(diff == np.array([0.0, 0.0, 0.0]))

@pytest.mark.parametrize("test_class",[InOutTestSystem,PythonInOutTestSystem])
def test_store_matrix(test_class):
    """Test that it is possible to store a matrix"""
    sim = Sim()
    sys = test_class()
    ref_mat = np.array([[1,0,0],
                        [0,2,0],
                        [0,0,3]])
    sys.store("input_output_matrix")
    sys.inputs.input_matrix =ref_mat
    sim.add_system(sys)
    sim.simulate(2,0.1)
    assert np.all(sys.res.input_output_matrix[-1]==ref_mat)

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_store_after_added_system(test_class):
    """Tests that it is possible to first add a system to a simulation and
    then store parameters.
    """
    sim = Sim()
    sys = test_class()
    sim.add_system(sys)
    sys.store("x")

    sim.simulate(10, 0.1)
    ressize = sys.res.x.size
    assert ressize == 101

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_time_store(test_class):
    """Check that the time is stored, and that the stored values includes
    both the beginning and the end of the simulation"""
    sim = Sim()
    sys = test_class()
    sim.add_system(sys)

    sim.simulate(2, 0.1)
    reftime = np.linspace(0,2,21)
    simtime = sys.res.time
    assert np.all(np.abs(simtime-reftime) <= np.finfo(float).eps)

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_continue_store(test_class):
    """Test that it is possible to continue a simulation without storing
    values twice.
    """
    sim = Sim()
    sys = test_class()
    sim.add_system(sys)

    sys.store("x")
    sim.simulate(1, 0.1)
    sim.simulate(1, 0.1)
    reftime = np.linspace(0,2,21)
    simtime = sys.res.time
    assert np.all(np.abs(simtime-reftime) <= np.finfo(float).eps)

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_interval_store(test_class):
    """Check that it is possible to change the interval between stored
    values.
    """
    sim = Sim()
    sys = test_class()
    sys.set_store_interval(0.2)
    sim.add_system(sys)

    sim.simulate(2, 0.1)
    reftime = np.linspace(0,2,11)
    simtime = sys.res.time
    assert np.all(np.abs(simtime-reftime) <= np.finfo(float).eps)
	
@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol,CompositeVanDerPol])
def test_midsim_store(test_class):
    """Check that it is possible to store a variable mid-simulation
    and that the result array is properly aligned with the timesteps
    and contains np.nan for locations where the variable was not stored.
    """
    sim = Sim()
    sys = test_class()
    
    sim.add_system(sys)
    sim.simulate(5, 1)
    sys.store("a")
    sim.simulate(5, 1)
    ares = sys.res.a
    assert np.all(np.isnan(ares[:6]))
    assert not np.any(np.isnan(ares[6:]))


if __name__ == "__main__":
    test_store_state(CompositeVanDerPol)
