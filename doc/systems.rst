Systems
=======
There are different systems that can be used in a simulation. The standard
building block is a c++ system, but there are also cython and python 
systems that fill the same role, but are much slower. All of these systems
are based on something called a commonsystem.

CommonSystem
------------
.. automodule:: pysim.commonsystem

.. autoclass:: pysim.commonsystem.CommonSystem
   :members:

.. autoclass:: pysim.commonsystem.Parameters

.. autoclass:: pysim.commonsystem.PysimVars

.. autoclass:: pysim.commonsystem.Results

CppSystem
---------
.. automodule:: pysim.cppsystem

.. autoclass:: pysim.cppsystem.Sys

CythonSystem
------------
.. automodule:: pysim.cythonsystem

.. autoclass:: pysim.cythonsystem.Sys

Connections
-----------
.. automodule:: pysim.connections

.. autoclass:: pysim.connections.Connections
   :members:




