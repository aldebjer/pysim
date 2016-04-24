"""Python Simulation package (PySim)
This package enables the user to create simulations based on partial
differential equations. The equations are defined in Systems and
solved in Simulations
"""
__copyright__ = 'Copyright (c) 2014 SSPA Sweden AB'

def test():
    """Runs all the tests for pysim"""
    import os
    import nose

    # find the directory where the test package lives
    from . import tests
    directory = os.path.dirname(tests.__file__)

    # get the name of the test package
    argv = ['nosetests', '--exe', directory]

    # run nose
    try:
        return nose.main(argv=argv)
    except SystemExit as e:
        return e.code
