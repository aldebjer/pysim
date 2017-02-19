Building PySim on Linux
=======================

To build PySim on Linux you need:
 * Python 3
 * Cython
 * NumPy
 * The Boost libraries
 * The Eigen c++ library

To run the test you will need
 * pytest
 * pytest-runner

And to build the documention you will need
 * sphinx
 * sphinx-rtd-theme

Getting the source
------------------
The source for pysim can be checked out from github by typing

.. code-block:: bash

   git clone https://github.com/aldebjer/pysim.git

which by default will clone the pysim repository into a pysim directory.

Installing the compiler and libraries
-------------------------------------
You might already have the compilers, boost and eigen libraries installed, but to make
sure type (on Ubuntu)

.. code-block:: bash

   sudo apt-get install build-essential libboost-all-dev libeigen3-dev

Installing the Python dependancies
----------------------------------
Python 3 is probably already installed, at least on the latest versions of Ubuntu.
The Numpy dependancy takes some time to compile so it is probably easiset to
install a pre compiled version by typing:

.. code-block:: bash

   sudo apt-get install python3-numpy

The rest of the dependancies can be installed through pip by moving to the pysim
directory and typing:

.. code-block:: bash

   pip3 install -r requirements.txt

Building PySim
--------------
To build pysim move to the pysim directory and type:

.. code-block:: bash

    python3 setup.py build_clib

and then

.. code-block:: bash

    python3 setup.py build_ext --inplace

This will build the c++ libraries and put the cython extensions side by side
with the python code.

Testing
-------
After all cython and c++ modules are built and placed in along the python modules
you can use PyTest to test the entire pysim package.

.. code-block:: bash

    python3 -m pytest

which should give you a result stating the number of tests, the time the testing
took, and a final "OK".

Building the documentation
--------------------------
To build the documentation go to the checked out pysim directort and types

.. code-block:: bash

    python3 setup.py build_sphinx
