"""Test the system ReadTextInput
"""
import tempfile
import numpy as np

from pysim.simulation import Sim
from pysim.systems import ReadTextInput

__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

def test_read_inputfile():
    """Tests that the system for reading text file values are ok
    It performs the test by creating a temporary file, filling the file with
    a numpy matrix. The file is read by the ReadTextInput system and the
    stored values are compared to the initial matrix.
    """
    timearray = np.arange(0.0, 100.0, 0.1)
    valuearray = timearray**2
    outmatrix = np.column_stack([timearray, valuearray])

    file = tempfile.NamedTemporaryFile(delete=False)
    np.savetxt(file, outmatrix, '%.6e')
    file.close()

    read_sys = ReadTextInput()
    read_sys.store("column_1")
    read_sys.pars.filename = file.name
    sim = Sim()
    sim.add_system(read_sys, "InputSystem")

    sim.simulate(100, 0.1)

    out = read_sys.res.column_1
    assert np.max(np.abs(out[3]-valuearray[3])) < 1e-15

if __name__ == "__main__":
    test_read_inputfile()
