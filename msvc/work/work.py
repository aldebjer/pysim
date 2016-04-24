import sys
import matplotlib.pyplot as plt

from pysim.simulation import Sim
from pysim.systems import VanDerPol


def main():
    """Test that the elapsed time is returned from the simulation"""
    sys = VanDerPol()
    sys.store("x")
    sys.store("y")
    sim = Sim()
    sim.add_system(sys)
    sim.simulate(20, 0.1)
    plt.plot(sys.res.time,sys.res.x)
    plt.plot(sys.res.time,sys.res.y)
    plt.show()

if __name__ == "__main__":
    sys.exit(int(main() or 0))
