Creating Systems
================
The building block of a PySim simulation is the *system*, of which there are
three variants.

 * C++ systems, which are build with a c++ compiler linked to the pysim
   libraries.
 * Python systems. They are much slower than the c++ systems, but also probably
   much faster to create.
 * Composite systems that consists of a collection of the systems above bundled
   together.
   
When using a system it looks and feels very much the same, regardless of which
the above types it belongs to. But the creation of the system is ofcourse 
different, and below we will go trough how to create a system of each type.


Creating a Python System
------------------------
To create a Python system you will inherit from the ``pysim.cythonsystem.Sys``
class and setup the system in its ``__init__`` function. If, for example,
you would like to create a system that simulates a
`Van der Pol Oscillator <https://en.wikipedia.org/wiki/Van_der_Pol_oscillator>`_
you would create a class like the one below.

.. code-block:: python

	from pysim.cythonsystem import Sys

	class VanDerPol(Sys):
		"""Simple example of a class representing a VanDerPol oscillator.
		"""
		def __init__(self):
			#declare states and inputs
			self.add_state("x", "dx")
			self.add_state("y", "dy")
			self.add_input("a")
			self.add_input("b")
			
			#set default values
			self.inputs.a = 1.0
			self.inputs.b = 1.0
			self.states.x = 1.0
			self.states.y = 0.0
			
It inherits from pysim.cythonsystem.sys, and sets up the system in the init
function. First two states *x* and *y* are declared together with their
respective derivatives. Then two scalar inputs, *a* and *b* are declared.
When these inputs and states are declared they are automatically added to 
the objects ``self.input`` and ``self.states`` structs. These structs can be
set and read from within the object as well as from outside of it.
Finally the default values for all inputs and states are set by setting
the values in these structs.

We now have a system with states and inputs, but we want the system to actually
do something. In particular we want the system to calculate the derivatives
based on the states and the inputs. The equations of a Van der Pol oscillator
can be written as

.. math::
  \begin{align}
  \dot{x} &= ax(b-y^2)-y \\
  \dot{y} &= x
  \end{align}
  
These equations should be evaluated for each time step in the simulation. This
evaluation is done in the ``do_step`` function which thus must be defined
in our class. To make the coded equations more readable in our function the 
auxiliary variables *a,b,x,y* are introduced and set to the respective inputs
and states. Then the derivatives are calculated.

.. code-block:: python

		def do_step(self,dummy):
			"""Perform a timestep by implmenting the VanDerPol equations"""
			
			a = self.inputs.a
			b = self.inputs.b
			x = self.states.x
			y = self.states.y

			self.ders.dx = a*x*(b-y*y)-y
			self.ders.dy = x

Thats it, we now have a complete system for simulating a Van der Pol oscillator.
The complete source code can be found at github `here <https://github.com/aldebjer/pysim-system-template/blob/master/pysim_system_template/systems/example_python_systems.py>`_.

Creating a Composite System
---------------------------
A Composite System is created by inhering from the
``pysim.compositesystem.CompositeSystem`` class and
setting up the system in its init function.

The systems that make up the compositesystem are added to it
with the function ``add_subsystem``, taking the system and the name of
the subsystem as inputs.To enable "outside" system to connect to and from the
composite systems *ports* are added to it with the functions ``add_input_port``
and ``add_output_port``. These functions take the name of the port (the name
of the input of the composite system), the name of the subsystem the port
shall be connected to internally, and the name of the input of that system.
The last argument is a description of the port

If you, for example want to create a composite system that is made up of two
systems, a mass-spring-damper system and a square wave system that will
drive the former system you create a class looking like below

.. code-block:: python

	class ControlledSpring(CompositeSystem):
		"""System with a mass-spring-damper driven by a square wave.
		
		The square wave is applied as a force acting on the mass,
		driving it up and down. The amplitude of the square wave can 
		be set with the input 'amp' and the position of the mass
		can be read from the output 'pos'.
		"""
		def __init__(self):

			#Create the two subsystems
			msd = MassSpringDamper()
			self.add_subsystem(msd,"msd")
			wave_sys = SquareWave()
			self.add_subsystem(wave_sys,"wave_sys")

			#connect the internal subsystems
			wave_sys.connections.add_connection("signal", msd, "f")

			#make parts of the subsystems available from outside this composite system
			self.add_input_port("amp","wave_sys","amplitude", "amplitude of wave")
			self.add_output_port("pos","msd","x1", "position")

The two subsystems are first created individually and connected as usual.
They are then added  to the composite system, and finally some of the inputs
and outputs are exposed to the composite system interface. 

This composite system will now behave as any other system from the outside.
It will have one input ``amp`` and one output ``pos``. 

Creating a C++ System
---------------------
While its very easy to create a composite or a python system c++ systems are a
little harder since they are compiled and the compiler needs links to pysim.
Until a better description is available the best way to create a c++ system is
to clone the template found at github in the 
`pysim-system-template <https://github.com/aldebjer/pysim-system-template>`_
repository and use that as a starting point. It contains a python system
as well as a c++ system that can be modified. Assuming you have installed
pysim the systems and clone the template systems it can be built by typing

.. code-block:: bash

	python setup.py build_ext

