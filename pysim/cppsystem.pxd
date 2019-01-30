from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from commonsystem cimport CommonSystemImpl
from commonsystem cimport CommonSystem

cdef extern from "CppSystem.hpp" namespace "pysim":

    cdef cppclass CppSystem(CommonSystemImpl):
        pass

cdef class Sys(CommonSystem):
    cdef CppSystem * _c_sys
    cdef bool _owner
    @staticmethod
    cdef Sys from_ptr(CppSystem* sys_p, string name, owner=*)
    cdef _setupParVar(self)
    cdef _get_subsystems(self)
