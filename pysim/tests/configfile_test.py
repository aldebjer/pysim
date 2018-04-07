"""Test that a config file can be created and read"""

import tempfile
import json

import pytest
import numpy as np

from pysim.simulation import Sim
from pysim.systems import VanDerPol, SquareWave, MassSpringDamper
from pysim.systems.python_systems import VanDerPol as PyVanDerPol
from pysim.tests.compositesystem_test import NestedCompositeSpring, CompositeSpring, ControlledSpring

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
    file = tempfile.NamedTemporaryFile()
    file.close()
    sim.save_config(file.name)

    file2 = open(file.name)
    simdict = json.load(file2)
    file2.close()
    assert simdict["systems"]["vanderpol"]["inputs"]["a"] == 1.234
    assert "x" in simdict["systems"]["vanderpol"]["stores"]

def test_store_config_composite_ports():
    """Test that it is possible to store the ports of a composite systems
    to a file.
    """

    sys = CompositeSpring()
    sim = Sim()
    sim.add_system(sys, "composite_root")

    # In this test a temp file is used, it should be deleted automatically
    # after the test.
    file = tempfile.NamedTemporaryFile()
    file.close()
    sim.save_config(file.name)

    file2 = open(file.name)
    simdict = json.load(file2)
    file2.close()
    print(simdict)

    ports = simdict["systems"]["composite_root"]["ports"]
    forceport = ports["in"]["force"]
    assert forceport["type"] == "scalar"
    assert forceport["value"] == 0
    assert forceport["description"] == "force acting on mass"
    assert forceport["connections"][0]["subsystem"] == "msd"
    assert forceport["connections"][0]["input"] == "f"

    posport = ports["out"]["position"]
    assert posport["type"] == "scalar"
    assert posport["value"] == 0
    assert posport["description"] == "Position"
    assert posport["connections"][0]["subsystem"] == "msd"
    assert posport["connections"][0]["output"] == "x1"

def test_store_connections():
    """Test that it is possible to store connections in a config file"""
    sim = Sim()
    wave_sys = SquareWave()
    wave_sys.inputs.amplitude = 50
    wave_sys.inputs.freq = 0.1
    sim.add_system(wave_sys,"wave_sys")

    msd = MassSpringDamper()
    msd.inputs.b = 80
    msd.inputs.m = 50
    msd.inputs.f = 0
    sim.add_system(msd,"msd")

    wave_sys.connections.add_connection("signal", msd, "f")

    # In this test a temp file is used, it should be deleted automatically
    # after the test.
    file = tempfile.NamedTemporaryFile()
    file.close()
    sim.save_config(file.name)

    file2 = open(file.name)
    simdict = json.load(file2)
    file2.close()
    print(simdict)

    cons = simdict["systems"]["wave_sys"]["connections"]
    assert cons[0] == ["signal", "msd", "f"]

def test_store_connections_composite():
    """Test that it is possible to store subsystem connections
    in a config file.
    """
    sim = Sim()
    sys = ControlledSpring()
    sim.add_system(sys, "controlled_spring")

    # In this test a temp file is used, it should be deleted automatically
    # after the test.
    file = tempfile.NamedTemporaryFile()
    file.close()
    sim.save_config(file.name)

    file2 = open(file.name)
    simdict = json.load(file2)
    file2.close()
    print(simdict)


    cons = simdict["systems"]["controlled_spring"]["subsystems"]["wave_sys"]["connections"]
    assert cons[0] == ["signal", "msd", "f"]

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
            "type": "VanDerPol",
            "connections": []
        },
       "vanderpol2": {
            "inputs": {
                "a": 1.0,
                "b": 3.456
            },
            "module": "pysim.systems.python_systems",
            "type": "VanDerPol",
            "connections": []
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

def test_load_connections():
    JSON_STRING = """
    {"systems": {
        "msd": {
            "connections": [],
            "inputs": {"b": 80.0, "f": 0.0, "k": 50.0, "m": 50.0},
            "module": "pysim.systems.defaultsystemcollection1",
            "stores": [],
            "type": "MassSpringDamper"
            },
        "wave_sys": {
            "connections": [["signal", "msd", "f"]],
            "inputs": {"amplitude": 50.0, "freq": 0.1,"phaseRad": 0.0},
            "module": "pysim.systems.defaultsystemcollection1",
            "stores": [],
            "type": "SquareWave"
            }
        }
    }
    """
    sim = Sim()
    file = tempfile.NamedTemporaryFile(delete=False, mode="w+")
    file.write(JSON_STRING)
    file.close()
    sim.load_config(file.name)
    sim.simulate(2, 0.1)
    res = sim.systems["msd"].states.x1
    assert np.abs(sim.systems["msd"].states.x1-0.3240587706226495) < 1e-10

def test_load_composite_connections():
    JSON_STRING = """
    {"systems": {
        "controlled_spring": {
            "type": "CompositeSystem",
            "module": "pysim.compositesystem",
            "connections": [],
            "inputs": {"amp": 0.0},
            "ports": {
                "in": {
                    "amp": {
                        "connections": [{"input": "amplitude", "subsystem": "wave_sys"}],
                        "description": "amplitude of wave",
                        "type": "scalar",
                        "value": 0.0
                        }
                    },
                "out": {
                    "out": {
                        "connections": [{"output": "x1", "subsystem": "msd"}],
                        "description": "position",
                        "type": "scalar",
                        "value": 0.0
                        },
                    "signal": {
                        "connections": [{"output": "signal", "subsystem": "wave_sys"}],
                        "description": "signal from wave",
                        "type": "scalar",
                        "value": 0.0
                        }
                    }
                },
            "stores": [],
            "subsystems": {
                "msd": {
                    "connections": [],
                    "inputs": {"b": 80.0, "f": 0.0, "k": 50.0, "m": 50.0},
                    "module": "pysim.systems.defaultsystemcollection1",
                    "stores": [],
                    "type": "MassSpringDamper"
                    },
                "wave_sys": {
                    "connections": [["signal", "msd", "f"]],
                    "inputs": {"amplitude": 50.0, "freq": 0.1, "phaseRad": 0.0},
                    "module": "pysim.systems.defaultsystemcollection1",
                    "stores": [],
                    "type": "SquareWave"
                    }
                }
            }
        }
    }
    """
    sim = Sim()
    file = tempfile.NamedTemporaryFile(delete=False, mode="w+")
    file.write(JSON_STRING)
    file.close()
    sim.load_config(file.name)
    sim.simulate(2, 0.1)
    assert np.abs(sim.systems["controlled_spring"].outputs.out-0.3240587706226495) < 1e-10

if __name__ == "__main__":
    #test_store_config_composite_ports()
    #test_store_connections()
    #test_store_connections_composite()
    #test_load_connections()
    test_load_composite_connections()
