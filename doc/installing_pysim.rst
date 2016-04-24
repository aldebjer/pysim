Installing PySim
================

Installing Python
-----------------
PySim is based on Python and is compatible with Python 3.3. The latest
version of Python 3.5 is 3.5.0. A Windows installer for can be 
downloaded through the link below.

https://www.python.org/ftp/python/3.5.0/python-3.5.0.exe

After Python is installed it is recommended to add the following lines 
to the system PATH (Assuming you installed python to C:\Python35): 

.. code-block:: bash

    C:\Python33\;C:\Python33\Scripts\

Installing pip
--------------

The easiest way to install PySim at SSPA is to use the pip command 
together with the SSPA in-house server. When installing
a package pip ensures that all dependencies of the package is installed 
as well. To use the SSPA internal server for packages you must modify the 
pip.ini file. This file might already exist, if not you can create it.
It should be found at the pip directory under your user directory.
For example

.. code-block:: bash

    C:\Users\lia\pip\pip.ini
    
Open the file and edit it to contain only the following lines

.. code-block:: bash

    [global]
    index-url = http://v012-buildmaster.sspa.local:8080/simple/

Save and close the file. You can now try to install a python package,
for example `NumPy <http://www.numpy.org/>`_, which handles matrixes
in Python, amongs others. To do this simply type:

.. code-block:: bash

    pip install numpy
    
If you want to make sure that you have the latest version of a package 
after it has been installed you can use the upgrade option:

.. code-block:: bash

    pip install --upgrade numpy
    
Finally, to get a list of packages and their respective versions you
can use the list command:

.. code-block:: bash

    pip list


Installing PySim
----------------
After installing pip installing and upgrading packages can be done from the
command line. To install PySim and all its extions type

.. code-block:: bash

    pip install pysim