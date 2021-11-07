"""Python Simulation package (PySim)
This package enables the user to create simulations based on partial
differential equations. The equations are defined in Systems and
solved in Simulations
"""
__copyright__ = 'Copyright (c) 2014-2017 SSPA Sweden AB'

__version__ = "2.4.0"

def test():
    """Runs all the tests for pysim"""
    import pytest

    try:
        return pytest.main("--pyargs pysim")
    except SystemExit as e:
        return e.code
