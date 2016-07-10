Running a Simulation
====================
To run a simulation you will need a Simulation, *Sim*, object. import
a Sim class and create an instance by typing

>>> import pysim.simulation
>>> sim = pysim.simulation.Sim

this instance will contain all the systems added to it. To add a system
type

>>> sim.add_system(system)

where *system* is the name of the system you want to add. You can also
specify a name by which the added system is known to the simulation.

>>> sim.add_system(system, "my_system")

When running a simulation it will solve the differential equations of the systems and
maintain any connections between systems. To run a standard simulation for 20
seconds with a timestep of 0.1 type

>>> sim.simulate(20, 0.1)

When the simulation is finished the equations of all systems have been solved,
and any variables designated have been stored. To solve the equations the
standard Runge Kutta 4 algorithm has been used.


Choosing a solver
-----------------
The default solver is the classic Runge Kutta 4, but it is possible to use
other solvers as well, some using adaptive step lengths and error control.
To use another solver the solver can be imported from the pysim.simulation
package. The solver should then be supplied to the simulate call. To use
the adaptive steplenght Cash Karp solver type

>>> from pysim.simulation import Cash_Karp
>>> solver = Cash_Karp()
>>> sim.simulate(20,0.1, solver)

the attentive reader now wonder why there is a timestep when the steplength
is adaptive. This timestep may be used as a starting point for the adaptive
step length, but it is also used as a time interval for storing values.
