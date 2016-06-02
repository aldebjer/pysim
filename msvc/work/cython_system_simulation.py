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
        self.statedict['x'][0] = 1
        self.statedict['x'][1] = 0
        self.stores = {'x':[]}

    def do_step(self,time):
        a = self.a
        b = self.b
        x = self.statedict['x'][0]
        y = self.statedict['x'][1]

        self.derdict['dx'][0] = a*x*(b-y*y)-y
        self.derdict['dx'][1] = x

    def do_storestep(self,time):
        self.stores['x'].append(self.statedict['x'].copy())


def main():
    """Test that the elapsed time is returned from the simulation"""
    sys = VanDerPol()
    sim = Sim()

    sim.add_cython_system(sys,"Cython1")
    sim.simulate(20, 0.1)
    print("ending")
    res = np.array(sys.stores["x"])
    print(res)

    import matplotlib.pyplot as plt
    plt.plot(res[:,0])
    plt.plot(res[:,1])
    plt.show()

if __name__ == "__main__":
    sys.exit(int(main() or 0))