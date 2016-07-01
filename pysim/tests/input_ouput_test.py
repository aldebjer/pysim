"""This module contains tests for the setting and reading of inputs and outputs
"""
import re

import pytest
import numpy as np

from pysim.simulation import Sim

from pysim.systems import Adder3D
from pysim.systems import VanDerPol
from pysim.systems import ReadTextInput
from pysim.systems import DiscretePID
from pysim.systems import RigidBody

__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

def test_invalid_inputname():
    """Test that an AttributeError is thrown if asking for a parameter that
    does not exist
    """
    sys = Adder3D()
    with pytest.raises(AttributeError):
        dummy = sys.inputs.xyxyxy

def test_invalid_outputname():
    """Test that an AttributeError is thrown if asking for a variable that
    does not exist
    """
    sys = Adder3D()
    with pytest.raises(AttributeError):
        dummy = sys.inputs.xyxyxy

def test_invalid_input_dimension():
    """Test that an ValueError is thrown if the number of elements in the
    parameter vector does not correspond with the number of values in the
    vector that it is being set to.
    """
    sys = RigidBody()
    array2d = np.array((0.0, 0.0))
    with pytest.raises(ValueError):
        sys.inputs.force = array2d

def test_invalid_vectorinput_type():
    """Test that an TypeError is thrown when trying to set a vector parameter
    to a scalar.
    """
    sys = Adder3D()
    with pytest.raises(TypeError):
        sys.inputs.input1 = 1

def test_invalid_input_type():
    """Test that an TypeError is thrown when trying to set a scalar parameter
    to a vector value.
    """
    sys = VanDerPol()
    array2d = np.array((0.0, 0.0))
    with pytest.raises(TypeError):
        sys.inputs.a = array2d

def test_par_init():
    """Tests that the par arrays are accessible and equal to 0"""
    sys = Adder3D()
    x = sys.inputs.input1
    refarray = np.array((0.0, 0.0, 0.0))
    assert np.array_equal(x, refarray)

def test_input_scalar_default():
    """Test that the default value of an input is accessible"""
    sys = VanDerPol()
    a = sys.inputs.a
    a_def1 = sys.inputs_default['a']
    assert a == a_def1
    sys.inputs.a = 2.0
    assert a == a_def1

def test_input_array_change():
    """Tests that it is possible to change input array"""
    sys = Adder3D()
    sys.inputs.input1 = (1.0, 2.0, 3.0)
    refarray = np.array((1.0, 2.0, 3.0))
    x = sys.inputs.input1
    assert np.array_equal(x, refarray)

def test_input_scalar_change():
    """Tests that it is possible to change input scalar"""
    sys = VanDerPol()
    testvalue = 1.234
    sys.inputs.b = testvalue
    x = sys.inputs.b
    assert np.array_equal(x, testvalue)

def test_input_string_change():
    """Tests that it is possible to change a text input"""
    sys = ReadTextInput()
    testvalue = "hej.txt"
    sys.inputs.filename = testvalue
    x = sys.inputs.filename
    assert x == testvalue

def test_input_string_default():
    """Tests the default value for text input"""
    sys = ReadTextInput()
    x = sys.inputs.filename
    assert x == "default.txt"

def test_getinputdoc():
    """Tests that the documentation of an input is returned."""
    sys = VanDerPol()
    s = sys.inputs.__repr__()

    comparestr = "{:>10}  {:>8.3}  {}".format("b", "1.0", "Scaling coefficient")
    found = re.search(comparestr,s,re.MULTILINE)
    assert found

def test_output_vector_init():
    """Tests that the output arrays are accessible and equal to 0"""
    sys = Adder3D()
    x = sys.outputs.output1
    refarray = np.array((0.0, 0.0, 0.0))
    assert np.array_equal(x, refarray)

def test_output_scalar_init():
    """Tests that the output scalars are accessible and equal to 0"""
    sys = DiscretePID()
    x = sys.outputs.outsig
    assert np.array_equal(x, 0)

def test_output_change():
    """Tests that it is possible to change input array.
    To make sure that the input is actually used in the simulation
    this is done by setting a input, run a simulation and
    see that the output value has changed.
    """
    sys = Adder3D()
    refarray1 = np.array((0.0, 0.0, 0.0))
    inputarray = (1.0, 2.0, 3.0)
    sys.inputs.input1 = inputarray
    x1 = sys.outputs.output1
    assert np.array_equal(x1, refarray1)
    sim = Sim()
    sim.add_system(sys)
    sim.simulate(1, 0.1)
    x2 = sys.outputs.output1
    assert np.array_equal(x2, inputarray)

def test_connected_system():
    """Check that it is possible to connect systems to each other 
    with boost vector outputs/inputs"""
    sys1 = Adder3D()
    sys2 = Adder3D()
    sys1.inputs.input1 = [1,2,3]
    sys1.connect("output1",sys2,"input1")
    sim = Sim()
    sim.add_system(sys1)
    sim.add_system(sys2)
    assert np.all(sys2.outputs.output1 == [0.0, 0.0, 0.0])
    sim.simulate(1,0.1)
    assert np.all(sys2.outputs.output1 == [1.0, 2.0, 3.0])


if __name__ == "__main__":
    test_connected_system()
