Building PySim on Windows
=========================

To build PySim on windows you need:
 * Python 3
 * Visual studio 2010 (express version works)
 * Cython
 * NumPy
 * nose
 
PySim can be built both using Setuptools and using Visual Studio. Building
with Setuptools allows for creating binary distributions and uploading 
these to the SSPA PyPi-server. Building with Visual Studio allows for 
step-debugging.

Install a compiler
------------------
Make sure that you have the visual studio compiler installed. This compiler
is used both for building with SetupTools and for building with the 
Visual Studio IDE. Since the official Python 3 is built with Visual Studio
compiler 2010 this is what PySim, as well as any other package for Python 3
need to be built with as well. The Visual C++ Express 2010 compiler
can be found at:
http://www.visualstudio.com/en-us/downloads#d-2010-express


Check out the Source
--------------------
Check out the source from the SSPA repository. If you want to make things 
easy with regards to paths then check out the source to "C:/Dev/pysims/pysim".
If you never was one to take the easy way then install it to any other
directory. To check out the source you can use `TortoiseSVN <http://tortoisesvn.net/>`_.

The source can be found at:
http://mimer.sspa.se/svn/sspa/pysim/trunk.

After the source has been checked out you should be able to find directories like
 "C:/Dev/pysims/pysim/msvc/" and a file "C:/Dev/pysims/pysim/setup.py". 

Building with setuptools
------------------------
To build with setuptools open a command prompt. Move to "C:/Dev/pysims/pysim/" 
and type 

.. code-block:: bash

    c:\python33\python setup.py build_clib
    c:\python33\python setup.py build_ext --inplace
    
You can now test the built packages by typing

.. code-block:: bash

    c:\python33\scripts\nosetests 
    
which should give you a result stating the number of tests, the time the testing
took, and a final "OK" printed.

If you want to try using pysim un a python prompt then type

.. code-block:: bash

    c:\python33\python
    
You can now do the tests from within the python environment by typing

>>> import pysim
>>> pysim.tests()


    


