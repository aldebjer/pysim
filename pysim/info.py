"""This module supplies info used to build extensions to pysim with cython
"""

import os
import os.path

__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

def get_include():
    """Gets the include file dir for cython extions to pysim"""
    currentdir = os.path.dirname(os.path.realpath(__file__))
    includedir = os.path.join(currentdir, "include")
    return includedir

def get_library_dir():
    """Gets the directory containg the library files needed to build
    cython extensions to pysim"""
    currentdir = os.path.dirname(os.path.realpath(__file__))
    libdir = os.path.join(currentdir, "lib")
    return [libdir]

def get_libraries():
    """Gets the name of the libraries used when building cython
    extensions to pysim"""
    return ["cppsystemlib",]
