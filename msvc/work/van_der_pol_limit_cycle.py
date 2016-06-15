from pysim.systems import VanDerPol
from pysim.simulation import Sim, Dormand_Prince_5, Cash_Karp
import numpy as np
import matplotlib.pyplot as plt
solver = Dormand_Prince_5()
#solver.relative_error = 1e-6

plt.axis('equal')
for my in [0.1,0.5,1,1.5]:
    sim = Sim()
    sys = VanDerPol()
    sys.inputs.a = my
    sys.store("x")
    sys.store("y")
    sim.add_system(sys)
    sim.simulate(200,0.01, solver = solver)
    plt.plot(sys.res.y[-800:],sys.res.x[-800:],lw = 3, c = 'black')

plt.show()
