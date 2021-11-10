---
layout: page
title: Developing PySim
excerpt: "Pysim lets you model and simulate and analyse dynamical systems"
modified: 2021-11-09T14:31:00.0-01:00
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
The tests are done automatically for each github push. They are done at github actions for Linux and macOS, 
and [AppVeyor](https://ci.appveyor.com/project/aldebjer/pysim) for Windows. The environments uses Python 3.7, 3.8, 3.9 to
build for both windows and Python. To
perform the tests there is a test collection stored in the tests folder. These can be run with pytest.

## Deploying
Deployments are done to PyPi where there is a registered package for [PySim](https://pypi.python.org/pypi/pysim). The
upload is done manually after each tag.
