import sys
import matplotlib.pyplot as plt
import numpy as np

from pysim.simulation import Sim
from pysim.cythonsystem import Sys

from collections import OrderedDict

class MySys(Sys):
    def __init__(self):
        self.add_state("x", "dx", 2)

    def dostep(self,time):
        print("x {}".format(self.statedict['x']))
        self.derdict['dx'][0] = self.statedict['x'][1]
        self.derdict['dx'][1] = 1.0



def main():
    """Test that the elapsed time is returned from the simulation"""
    sys = MySys()
    sim = Sim()

    sim.add_cython_system(sys,"Cython1")
    sim.simulate(20, 0.1)
    print("ending")


if __name__ == "__main__":
    sys.exit(int(main() or 0))