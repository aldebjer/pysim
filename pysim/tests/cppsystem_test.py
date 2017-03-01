import pytest

from pysim.systems import InOutTestSystem
from pysim.simulation import Sim

class InheritanceTester(InOutTestSystem):
    pass

def test_inheritance():
    """Test that it is possible to inherit from a cpp system"""
    sys = InheritanceTester()
    assert sys.states.state_scalar == 1.23


if __name__ == "__main__":
    test_inheritance()
