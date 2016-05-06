"""This module contains tests for the store function of pysim
"""
import re

from pysim.systems import VanDerPol

__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

def test_setstate():
    """Test that it is possible to set and receive a state variable.
    """
    sys = VanDerPol()
    sys.states.x = 1.0
    assert sys.states.x == 1.0

def test_getstatedoc():
    """Tests that the documentation of a state is returned."""
    sys = VanDerPol()
    s = sys.states.__repr__()
    print(s)

    comparestr = "{:>10}  {:>8.3}  {}".format("x", "1.0", "Position coordinate")
    found = re.search(comparestr,s,re.MULTILINE)
    assert found
