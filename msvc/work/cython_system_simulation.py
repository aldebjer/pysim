import sys

import numpy as np
import matplotlib.pyplot as plt

from pysim.simulation import Sim
#from pysim.systems.python_systems import VanDerPol as PyVanDerPol
from pysim.systems import VanDerPol

def main():
    """Test that the elapsed time is returned from the simulation"""
    vdp = VanDerPol()
    vdp.store("x")
    vdp.store("y")
    sim = Sim()

    sim.add_system(vdp,"Cython1")
    sim.simulate(20, 0.1)
    plt.plot(vdp.res.x)
    plt.plot(vdp.res.y)
    plt.show()

if __name__ == "__main__":
    sys.exit(int(main() or 0))