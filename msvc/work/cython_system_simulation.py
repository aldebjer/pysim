import sys

import numpy as np
import matplotlib.pyplot as plt

from pysim.simulation import Sim


from collections import OrderedDict

from pysim.systems.python_systems import VanDerPol

def main():
    """Test that the elapsed time is returned from the simulation"""
    sys = VanDerPol()
    sys.store("x")
    sim = Sim()

    sim.add_cython_system(sys,"Cython1")
    sim.simulate(20, 0.1)

    plt.plot(sys.res.x[:,0])
    plt.plot(sys.res.x[:,1])
    plt.show()

if __name__ == "__main__":
    sys.exit(int(main() or 0))