import sys
import matplotlib.pyplot as plt

from pysim.simulation import Sim
from pysim.cythonsystem import Sys
from pysim.systems import VanDerPol

def main():
    """Test that the elapsed time is returned from the simulation"""
    sys = Sys()
    #oldsys = VanDerPol()

    sim = Sim()

    #sim.add_system(oldsys, "oldsys")
    sim.add_cython_system(sys,"Cython1")
    sim.simulate(20, 0.1)
    print("ending")


if __name__ == "__main__":
    sys.exit(int(main() or 0))