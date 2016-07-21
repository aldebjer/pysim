Installing PySim
================

Installing on Windows
---------------------
PySim is based on Python and is compatible with Python 3. The latest
version of Python 3 is 3.5.2. A Windows installer for can be
downloaded through the link below.

https://www.python.org/ftp/python/3.5.2/python-3.5.2.exe

Once you have python installed you have some options on how to install
PySim. The easiest one is to use pip. If you want to do this you
can simply type

.. code-block:: bash

    pip install pysim

And the wheel for pysim and all its dependancies will be downloaded

Installing on Ubuntu
--------------------
Python 3 is already installed. PySim is tested
on Xenial (Python 3.5).

To install PySim on Ubuntu you will need to build it. To do this you
need gcc, and the boost libraries. These can be installed by

.. code-block:: bash

    sudo apt-get install libboost-all-dev

once this is done you can install PySim and its requirements by typing

.. code-block:: bash

    pip3 install pysim

Installing on Mac OS
--------------------
PySim must be run on Python 3 and does not work on the default Mac OS
version (Python 2). You can download Python 3 for Mac OS here:

https://www.python.org/downloads/mac-osx/
