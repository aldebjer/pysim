---
layout: post
title: PySim Release 2.1.0
excerpt: "This new release adds a third type of system, the composite system"
modified: 2016-10-31T23:50:00+01:00
categories: blog
tags: [release]
comments: false
share: true
---
# PySim Release 2.1.0
This release is mostly about the added composite systems introduced. These type of systems can consist of other c++ or python systems that are nested under it. These "subsystems" are not visible to the outside of the composite system but are communicating through ports of the composite system. There is more information on how to create these type of systems in the documentation, please have a look.

Apart from the composite system another new functionality is the possibility to connect a element of a vector to a scalar input of another system.


## Install
The source distribution as well as wheel files for windows can be found on [PyPi](https://pypi.python.org/pypi?:action=display&name=pysim&version=2.0.0). You can also download the source directly from github and follow the [install instructions](http://pysim.readthedocs.io/en/latest/installing_pysim.html) at readthedocs.

## Development
There has been some major refactoring in between release 2.0.0 and 2.1.0. There is now a parent class called "commonsystem" that contains the functionality that is shared between the cythonsystem, cppsystem and the compositesystem. There is also a new Variable class that contains the functionality of the inputs and outputs. This will decrease the code that was previously repeated between these functions.

A release branch for this release has been created at github and is named *2-1-stable*, and a tag for release 2.1.0 has been taken from that branch. Any future bugfixes will be made in the *master* branch and merged into the release branch, from which a tag for the next 2.1.x release will be taken. Once the time has come for a 2.2.0 release a new release branch will be taken from *master* in the same manner.

## Whats next?
For the next part i will focus on better documentation, perhaps a slideshow introducing pysim. I would also love to have some more systems implemented. Right now there are a couple of example systems with varying quality and usability. In order to use PySim you pretty much have to implement some systems yourself. So, more and better organised systems!
