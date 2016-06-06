---
layout: page
title: Developin PySim
excerpt: "Pysim lets you model and simulate and analyse dynamical systems"
modified: 2016-06-05T14:31:00.0-01:00
---
One of the main benefits of putting a project at github is that multiple persons all over the world
can collaborate to make the project as good as possible. So any contributions are very welcome. There
are a lot of areas in which PySim can be improved substantially. Please get in touch with me if you would
like to contribute! No contributions are too small.

## Coding
The code is currently placed at [Github](https://github.com/aldebjer/pysim). It is a mix of Python and C++
which is glued together with Cython. The main principle is to do the heavy computational stuff in C++ and
the configurations and evaluations in Python. Thus the C++ boost libraries are used to solve the differential
equations that are described in C++ classes. Before and after the simulations Cython is used to enable Python
access to these classes.

## Testing
The tests are done automatically for each github push. They are done at [Jenkins CI](https://travis-ci.org/aldebjer/pysim) for Linux
and [AppVeyor](https://ci.appveyor.com/project/aldebjer/pysim) for Windows. The environments use Python 3.5 to
build for windows but Python 3.4 to build for Linux. This is due to the default setups of the build machines. To
perform the tests there is a test collection stored in the tests folder. These can be run either with nosetests
or pytest.

## Deploying
Deployments are done to PyPi where there is a registered package for [PySim](https://pypi.python.org/pypi/pysim). The
upload is done manually after each tag.
