---
layout: post
title: Introducing Python Systems
excerpt: "Python systems are now implemented"
modified: 2016-07-00T16:57:25-04:00
categories: blog
tags: [milestones]
comments: false
share: true
---

# Python Systems
Python systems are now implemented! This means that it is possible to write your new system in Python first, to verify its functionality. Once you have a proof of concept it is still probably a good idea to translate it to a complete c++ system if you are going to use it again. I have done some simple tests that show that a python system is about 200 times slower than a c++ system, just by calling python from c++. If you are doing calculations in python instead of c++ in your system, that difference in speed will add to it. There is also a way to increase the performance without going to c++ and that is by creating a cython system. I have not yet spent any time looking into the performance but it will probably be much better than the python system.

### Implementation
The Python systems are actually inheriting from a cython system. So in the same way that it is possible to create a python system it is also possible to create a cython system. Anyway, the cython system is inheriting from something called a "CommonSystem". This is a base class that is used by both the cython systems and the trusty old c++ systems. This change means that some refactoring was done to the c++ systems as well, and I believe that they turned up much nicer in the process, as well.

### Documentation
The documentation of the new python and cython systems needs to be created, as is the documentation of the new "parameters" (see below). This is the next thing to do for me, and perhaps the last thing before a 2.0 release.

### What else is new?
A lot of small changes and bug fixes have been made, such as the introduction of namespaces in c++ systems. The change that is most important to users of pysim is probably the introduction of parameters. These are designed to be more flexible than "inputs" in what types are allowed. They are not, however, designed for connecting to other systems in the way that the inputs/outputs are used. So far there is two type of parameters: strings and maps of doubles.
