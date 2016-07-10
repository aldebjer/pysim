Building PySim on Windows
=========================

To build PySim on windows you need:
 * Python > 3.5
 * Visual studio 2015 (community version works)
 * Cython
 * NumPy
 * PyTest

PySim can be built both using Setuptools and using Visual Studio. Building
with Setuptools allows for creating binary distributions and uploading
these to PyPi. Building with Visual Studio allows for
step-debugging.

Install a compiler
------------------
Make sure that you have the visual studio compiler installed. This compiler
is used both for building with SetupTools and for building with the
Visual Studio IDE. Since the official Python 3 is built with Visual Studio
compiler 2015 this is what PySim, as well as any other package for Python 3
need to be built with as well.


Check out the Source
--------------------
The source can be found at:
https://github.com/aldebjer/pysim.git

After the source has been checked out you should be able to find directories like
 "pysim/msvc/" and a file "pysim/setup.py".

Building with setuptools
------------------------
To build with setuptools open a command prompt. Move to the pysim directory, e.g. "C:/Dev/pysims/pysim/"
and type

.. code-block:: bash

    python setup.py build_ext --inplace

You can now test the built packages by typing

.. code-block:: bash

    python -m pytest

which should give you a result stating the number of tests, the time the testing
took, and a final "OK" printed.

If you want to try using pysim un a python prompt then type

.. code-block:: bash

    python

You can now do the tests from within the python environment by typing

>>> import pysim
>>> pysim.tests()
