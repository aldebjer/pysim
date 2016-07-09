---
layout: page
title: Use Pysim
excerpt: "Pysim can be used with existing systems, or to model new systems"
modified: 2016-06-06T12:23:00.0-01:00
---
PySim is used with Python in which a simulations is set up, simulated and analysed. To see
how you would simulate a simple Van der Pol oscillator have a look at the following example: 

{% highlight python %}
from pysim.simulation import Sim
from pysim.systems import VanDerPol
import matplotlib.pyplot as plt

#Create a Simulation
sim = Sim()

#Create, setup and add a system to simulation
sys = VanDerPol()
sys.store("x")
sim.addSys(sys)

#Simulate and plot the results
sim.simulate(20,0.1) 
plt.plot(sys.res.x)
plt.show()
{% endhighlight %}

### Connected systems and other solvers
Of course much more can be done, the example above uses only one system while most simulations would
use several systems connected. It is also possible to use other solvers instead of the default Runge Kutta 4
solver used above. The following solvers are currently available:

* Runge Kutta 4
* Cash Karp
* Dormand Prince 5

### Creating own systems
It is also possible to create your own systems in either C++ or Python. All systems are representations of
differential equations, having a state as well as inputs and outputs. 

### Documentation
The complete documentation for PySim can be found at [**http://pysim.rtfd.io**](http://pysim.rtfd.io)


