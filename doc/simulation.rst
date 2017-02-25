simulation
==========

Introduction
------------
.. automodule:: pysim.simulation


Simulation
----------

.. autoclass:: pysim.simulation.Sim
   :members:

Solvers
-------
There is currently three solvers that can be used by the Sim class to 
solve the differential equations. The default is the Runge Kutta 4 
algorithm which uses a fixed steplength. But there is two addional
solvers with variable step length that can be used.

.. autoclass:: pysim.simulation.Runge_Kutta_4

.. autoclass:: pysim.simulation.Cash_Karp

.. autoclass:: pysim.simulation.Dormand_Prince_5


