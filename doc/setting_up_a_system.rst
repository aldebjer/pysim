Setting up a system for simulation
==================================

Before running a simulation with a system it is possible to configure the
system. The configuration generally involves:

* Setting parameters for the simulation
* Setting start values for the states
* Selecting what states and variables to store
* Connecting the system to other systems involved in the simulation

Setting parameters
------------------
Parameters are set by assigning a value to the parameter. If the
parameter is a vector the input must also be a list, tuple or numpy
vector, otherwise an exception will be thrown.

>>> sys.pars.x = 2.0

or for a vector:

>>> sys.pars.v = (2.0,1.0,0.0)

Setting start values
--------------------
Start values can be set by setting the state or input variable of the system

>>> sys.state.x = 2.0
>>> sys.inputs.a = 2.0

This value is then used as the first value for the state x.


Storing
-------
A state or a variable can be stored during each step of the simulation. To
do this call the store command of the system, with the name of the state or
variable to be stored as an argument.

>>> sys.store("x")

Connecting Systems
------------------
The Systems can be connected so that the output from one system is used as
input to another. Say for example that you want to use a square wave to
drive the force applied to a mass in the mass spring damper system, then
the following code would be used

.. code-block:: python

    from pysim.simulation import Sim
    from pysim.systems import MassSpringDamper,SquareWave
    import matplotlib.pyplot as plt

    #Create Simulation
    sim = Sim()

    #Setup Square Wave system
    wavesys = SquareWave()
    wavesys.store("signal")
    wavesys.pars.freq = 0.2
    wavesys.pars.amplitude = 100

    #Setup a Mass Spring Damper system
    sys = MassSpringDamper()
    sys.store("x1")
    sys.pars.b = i
    sys.pars.f = 0

    #Connect variable "signal" from Square wave system to parameter f in
    #MassSpringDamper system.
    wavesys.connect("signal",sys,"f")

    #Add both systems to simulation
    sim.addSys(wavesys)
    sim.addSys(sys)

    #Simulate and plot the results
    sim.simulate(50, 0.01)
    plt.plot(sys.res.x1,'-')
    plt.plot(wavesys.res.signal/50)
