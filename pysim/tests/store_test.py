"""This module contains tests for the store function of pysim
"""
import numpy as np

import pytest

from pysim.simulation import Sim
from pysim.systems import VanDerPol, SquareWave, RigidBody
from pysim.systems.python_systems import VanDerPol as PythonVanDerPol

__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol])
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

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol])
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

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol])
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

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol])
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

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol])
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

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol])
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

@pytest.mark.parametrize("test_class",[VanDerPol,PythonVanDerPol])
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
