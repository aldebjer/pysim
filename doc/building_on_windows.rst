Building PySim on Windows
=========================

To build PySim on windows you need:
 * Visual studio 2015 (community version works)
 * Boost Libraries
 * Python > 3.5
 * Cython
 * NumPy
 * PyTest

Install a compiler
------------------
Make sure that you have the visual studio compiler installed. This compiler
is used both for building with SetupTools and for building with the
Visual Studio IDE. Since the official Python 3 is built with Visual Studio
compiler 2015 this is what PySim, as well as any other package for Python 3
need to be built with as well.

Install Boost
-------------
You can download the Boost libraries from http://www.boost.org/.

For PySim to find the boost libraries there must be a environment variable
named ``BOOST_ROOT`` that is set to the directory where Boost is installed. This
directory should have the boost include directory as subfolders.

Check out the Source
--------------------
The source can be found at: https://github.com/aldebjer/pysim.git

Check out the source to a source directory, e.g. ``C:/Dev/pysim``

Install Python Dependancies
---------------------------
The python requirements are collected in a file called requiements.txt. To
install these requirements use pip:

.. code-block:: bash

    pip install -r requirements.txt

Building
--------
PySim can be built either using Setuptools or using the Visual Studio IDE.
Building with Setuptools allows for creating binary distributions and uploading
these to PyPi. Building with Visual Studio allows for mixed python-c++
step-debugging. Both methods are described below.


Building with Setuptools
^^^^^^^^^^^^^^^^^^^^^^^^
To build with setuptools open a command prompt and move to the pysim directory,
e.g.``C:/dev/pysim/``. To build the c libraries type:

.. code-block:: bash
    python setup.py build_clib

After that we build the the c++ and cython
modules and place them besides the python module by typing:

.. code-block:: bash
    python setup.py build_ext --inplace

Building with the Visual Studio IDE
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
There are solution and project files for visual studio in the ``MSVC``
directory. Open the file ``msvc/pysim.sln`` with visual studio, select the
release build and choose build all.


Testing
-------
After all cython and c++ modules are built, either with setuptools or with
the visual studio ide, and placed in along the python modules you can use
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
