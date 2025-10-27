from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from pysim.commonsystem cimport CommonSystemImpl
from pysim.commonsystem cimport CommonSystem

cdef extern from "CppSystem.hpp" namespace "pysim":

    cdef cppclass CppSystem(CommonSystemImpl):
        pass

cdef class Sys(CommonSystem):
    cdef CppSystem * _c_sys
    cdef _setupParVar(self)
