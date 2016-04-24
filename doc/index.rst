PySim Documentation
===================
PySim is a python extension that enables the user to create 
simulations based on partial differential equations. 

The differential equations are collected in systems. Each system
contains a state and its time-derivative. It can contain inputs to the 
system, e.g. parameters to be set by the users. It can also contain outputs,
e.g. non-state variables that should be stored during the simulation and then
displayed. Before a simulation systems can be connected to each other, so that
the ouput from one system is the input to another. This enables the user to 
create complex simulations based on individual components.

Installing PySim
----------------
PySim can be be installed from within SSPA by using pip. Simply type

.. code-block:: bash

    pip install pysim

For further instructions on how to install PySim and its prerequisites see:

.. toctree::
   :maxdepth: 1

   installing_pysim.rst

Using PySim
-----------
To setup a simulation import a Sim object from the Simulation
package. Then add the system that should be simulated, set any
parameters, decide what variables or states that should be stored
during the simulation, and then run the simulation. After it is 
finished the results can be inspected using for example matplotlib.

.. code-block:: python

    from pysim.simulation import Sim
    from pysim.systems import VanDerPol
    import matplotlib.pyplot as plt
    
    #Create Simulation
    sim = Sim()
    
    #Create, setup and add system to simulation
    sys = VanDerPol()
    sys.store("x")
    sim.addSys(sys)
    
    #Simulate and plot results
    sim.simulate(20,0.1)
    x = sys.res.x
    plt.plot(x)
    plt.show()
    
Which will result in:
    
.. image:: /images/VanDerPolPlot.png
    
For further more details on how to set up a system see:

.. toctree::
   :maxdepth: 1

   setting_up_a_system.rst
   analyzing_results.rst

Prebuilt Systems
----------------
PySim is build to be extended with custom systems. There are however some
prebuilt systems included in the PySim, mainly for ilustration and
demonstration purposes. The systems are listed below.

.. toctree::
   :maxdepth: 2

   systems.rst

Developing PySim
================
PySim can be built and installed using distutils. The requirements are
NumPy. If you want to run the tests you will also need Nose. 

Building PySim
--------------

.. toctree::
   :maxdepth: 2
   
   building_on_windows
   building_on_linux
    

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

