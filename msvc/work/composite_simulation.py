import matplotlib.pyplot as plt

from pysim.compositesystem import CompositeSystem
from pysim.systems import VanDerPol
from pysim.simulation import Sim

vdp = VanDerPol()
cs = CompositeSystem()
cs.add_subsystem(vdp,"vdp")
cs.add_input_port("atot","vdp","a","testing a")
#cs.inputs.atot = 6.789
#print(vdp.inputs.a)
sim = Sim()
sim.add_system(cs,"composite")
#sim.add_system(vdp,"composite")
vdp.store("x")
sim.simulate(20,0.1)
plt.plot(vdp.res.x)
plt.show()
