from pysim.compositesystem import CompositeSystem
from pysim.systems import VanDerPol

vdp = VanDerPol()
cs = CompositeSystem()
cs.add_subsystem(vdp,"vdp")
cs.add_input_port("atot","vdp","a","testing a")
cs.inputs.atot = 6.789
print(vdp.inputs.a)
