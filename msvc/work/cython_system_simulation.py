import sys

import numpy as np

from pysim.simulation import Sim
from pysim.cythonsystem import Sys

from collections import OrderedDict

class VanDerPol(Sys):
    def __init__(self):
        self.add_state("x", "dx", 2)
        self.a = 1.0
        self.b = 1.0
        self.states.x = [1.0, 0.0]

    def do_step(self,time):
        a = self.a
        b = self.b
        x = self.statedict['x'][0]
        y = self.statedict['x'][1]

        self.derdict['dx'][0] = a*x*(b-y*y)-y
        self.derdict['dx'][1] = x


def main():
    """Test that the elapsed time is returned from the simulation"""
    sys = VanDerPol()
    sys.store("x")
    sim = Sim()

    sim.add_cython_system(sys,"Cython1")
    sim.simulate(20, 0.1)
    print("ending")

    import matplotlib.pyplot as plt
    plt.plot(sys.res.x[:,0])
    plt.plot(sys.res.x[:,1])
    plt.show()

if __name__ == "__main__":
    sys.exit(int(main() or 0))