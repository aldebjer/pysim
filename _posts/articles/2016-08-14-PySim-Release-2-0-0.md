---
layout: post
title: PySim Release 2.0.0
excerpt: "The first public open source release of PySim"
modified: 2016-08-14T23:50:00+01:00
categories: blog
tags: [release]
comments: false
share: true
---
# PySim Release 2.0.0
There is a first public open source release of pysim, 2.0.0. I have been very reluctant up to the last minute to actually do the release since I would hate to break backward compatability after this release. But there had to be a release eventually, and here it is. Since this is the first public release there will be no changelog, but the most work done lately has been in three areas:
* Adding Python and Cython systems
* More flexible inputs, *parameters*
* Compatability with Linux and Mac OS
* Integration with Travis-CI and Appveyor
* Improved documentation, integrated with readthedocs
* Improved Binary Compatibility

A template for creating your own pysim-systems has also been created in a separate repository, https://github.com/aldebjer/pysim-system-template. If you want to create systems in c++ it is probably easiest to clone that repo and use it as a starting point for your systems. If you think they are useful for others please let me know and we can include the systems into the standard pysim.

PySim will use semantic versioning, which means that 2.0.x will only be bugfixes for this release, while new features will be introduced in release 2.1.0. 

## Install
The source distribution as well as wheel files for windows can be found on [PyPi](https://pypi.python.org/pypi?:action=display&name=pysim&version=2.0.0). You can also download the source directly from github and follow the [install instructions](http://pysim.readthedocs.io/en/latest/installing_pysim.html) at readthedocs.

## Development
A release branch for this release has been created at github and is named *2-0-stable*, and a tag for release 2.0.0 has been taken from that branch. Any future bugfixes will be made in the *master* branch and merged into the release branch, from which a tag for the next 2.0.x release will be taken. Once the time has come for a 2.1.0 release a new release branch will be taken from *master* in the same manner.

## Whats next?
I would really like to create some way of nesting systems, so that you can create a new system out of existing ones, and make more and more complex simulations. I have some ideas on how to do it, but they need some more thought before implemented. Besides this I would love to have some more systems implemented. Right now there are a couple of example systems with varying quality and usability. In order to use PySim you pretty much have to implement some systems yourself. So, more and better organised systems!
