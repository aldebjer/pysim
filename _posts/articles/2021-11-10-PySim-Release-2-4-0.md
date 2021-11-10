---
layout: post
title: PySim Release 2.4.0
excerpt: "Running against Python 3.9"
modified: 2021-11-10T22:33:00+01:00
categories: blog
tags: [release]
comments: false
share: true
---
# PySim Release 2.4.0
This is the first release for quite some time. There are no new features and the only change in code is to make it compatible with newer compilers and new Python versions. The release has been tested against:

* Python 3.7
* Python 3.8
* Python 3.9

at the following platforms
* Windows 10
* macOS
* Linux

There has also been some infrastructure changes for the CI/CD system. Github Actions are now used to build, test and deploy the macOS and Linux versions, replacing Travis. The windows tests are still running at Appveyor. Finally the web page has been changed for both this blog and the documentation, it is now https://pysim.org and the documentation can be found at https://doc.pysim.org.

## Install
The source distribution as well as wheel files for macOS and Linux can be found on [PyPi](https://pypi.python.org/pypi?:action=display&name=pysim&version=2.4.0). You can also download the source directly from github and follow the [install instructions](https://doc.pysim.org/tutorial.html#installing-pysim).

