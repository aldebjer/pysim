---
layout: post
title: Short term goals
excerpt: "Some discussion on the short term goals of Pysim."
modified: 2016-06-06T14:17:25-04:00
categories: articles
tags: [milestones]
comments: true
share: true
---

# Where is Pysim right now?

As mentioned in the previous article Pysim has been used in-house at SSPA for some years now, with a revision number larger 
than 1 but smaller than two. Before launching Pysim to the world there are some breaking changes both in architecture and
api that I would like to do. Thus *the first public Pysim will be versioned as Pysim 2.0*. In the days were 0.1 is the new 1.0 this
might seem strange and overconfident, but it is the only way to not mess up dependancies in-house at SSPA.

So what breaking changes do I want to do? 

## Python Systems

The first and perhaps most important is to allow for Python systems. Right now when you
create a system it has to be in C++. But sometimes it is nice to be able to prototype with the quickness of Python, and sacrifice 
the speed of C++. When everything is working, or at least seems feasible to work in Python the system can be implemented in C++.

## More flexible inputs

All the individual systems communicate with something called "inputs" and "outputs". The output of one system can be the input to
another system, making it possible to build simulations of multiple systems. Normally the inputs/outputs are either doubles or arrays
of doubles. This is fine when running the simulation, but when setting up a system more flexibility is needed. It would sometimes
be nice to be able to pass a python dict to a system for example. But it would not be very common to pass dicts between systems during
the simulation. *So a new type of variable for systems are needed, "parameters"*. The parameters of a system can only be set before
the simulation, and cannot be connected to other systems. So in this regard it is more limited. But the type of parameters could
be much more flexible, for example dicts, strings, variable length arrays, etc.

## Automatic tests

Pysim has used in house infrastructure at SSPA to perform the tests. Since it now is open source it is natural to do these tests
at services like Travis CI (for Linux and Mac OS) and AppVeyor (for Windows). Most of this has been done, but some things remain,
such as builds for Mac OS. I have built Pysim under Mac OS for testing purposes, but there is not yet automatic building and tests.

# Where will Pysim be?

My ambition is to release a beta of Pysim 2.0.0 at the first of July, even if these changes are not yet implemented. But with any luck
they will be!

