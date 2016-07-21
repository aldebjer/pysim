Building PySim on Mac OS
========================

To build PySim on Mac you need:
 * gcc
 * The boost libraries
 * Python 3
 * NumPy
 * Cython

Install a compiler and c++ dependancies
---------------------------------------
For instructions how to install gcc and python please look at these instructions
http://docs.python-guide.org/en/latest/starting/install/osx/

Once you have gcc and python installed you will also need the boost libraries. To
get these by using brew type:

.. code-block:: bash

   brew install boost

Get the PySim source
--------------------

To check out the master branch of the source from github types

.. code-block:: bash

   git clone https://github.com/aldebjer/pysim.git

Install the python dependancies
-------------------------------
The python requirements are collected in a file called requiements.txt. To
install these requirements use pip:

.. code-block:: bash

   pip3 install -r requirements.txt

Build PySim
-----------
you should now be able to build the c++ and cython extensions and place them
along the python code by typing:

.. code-block:: bash

   python3 setup.py build_ext --inplace


Testing
-------
After all cython and c++ modules are built and placed in along the python modules you can use
PyTest to test the entire pysim package.

.. code-block:: bash

   python -m pytest

which should give you a result stating the number of tests, the time the testing
took, and a final "OK" printed.

If you want to try using pysim with a python prompt then type

.. code-block:: bash

   python

You can now do the tests from within the python environment by typing

>>> import pysim
>>> pysim.tests()
