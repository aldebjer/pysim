"""Test that a config file can be created and read"""

import tempfile
import json
import pytest

from pysim.simulation import Sim
from pysim.systems import VanDerPol
from pysim.systems.python_systems import VanDerPol as PyVanDerPol
from pysim.tests.compositesystem_test import NestedCompositeSpring

__copyright__ = 'Copyright (c) 2014-2017 SSPA Sweden AB'

@pytest.mark.parametrize("test_class",[VanDerPol,PyVanDerPol])
def test_store_config(test_class):
    """Test that it is possible to store the simulation to a file.
    """
    sys = test_class()
    sim = Sim()
    sim.add_system(sys)
    sys.inputs.a = 1.234
    sys.store("x")

    # In this test a temp file is used, it should be deleted automatically
    # after the test.
    file = tempfile.NamedTemporaryFile(delete=False)
    file.close()
    sim.save_config(file.name)

    file2 = open(file.name)
    simdict = json.load(file2)
    file2.close()
    assert simdict["systems"]["vanderpol"]["inputs"]["a"] == 1.234
    assert "x" in simdict["systems"]["vanderpol"]["stores"]

def test_store_config_composite():
    """Test that it is possible to store the simulation with composite systems
    to a file.
    """
    sys = NestedCompositeSpring()
    sim = Sim()
    sim.add_system(sys, "composite1")
    sys.inputs.force = 1.234
    sys.store("position")

    # In this test a temp file is used, it should be deleted automatically
    # after the test.
    file = tempfile.NamedTemporaryFile(delete=False)
    file.close()
    sim.save_config(file.name)

    file2 = open(file.name)
    simdict = json.load(file2)
    file2.close()
    assert simdict["systems"]["composite1"]["inputs"]["force"] == 1.234
    assert "position" in simdict["systems"]["composite1"]["stores"]

def test_load_config():
    """Tests the loading of a system configuration from file"""

    configstring = """{
    "systems": {
        "vanderpol1": {
            "inputs": {
                "a": 1.234,
                "b": 1.0
            },
            "module": "pysim.systems.defaultsystemcollection1",
            "type": "VanDerPol"
        },
       "vanderpol2": {
            "inputs": {
                "a": 1.0,
                "b": 3.456
            },
            "module": "pysim.systems.python_systems",
            "type": "VanDerPol"
        }
    }
}
"""

    sim = Sim()
    file = tempfile.NamedTemporaryFile(delete=False, mode="w+")
    file.write(configstring)
    file.close()
    sim.load_config(file.name)
    assert sim.systems["vanderpol1"].inputs.a == 1.234
    assert sim.systems["vanderpol2"].inputs.b == 3.456

if __name__ == "__main__":
    test_store_config_composite()
