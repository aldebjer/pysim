from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from commonsystem cimport CommonSystemImpl
from commonsystem cimport CommonSystem

cdef extern from "CythonSystemImpl.hpp" namespace "pysim":
    cdef cppclass CythonSystemImpl(CommonSystemImpl):
        void* sysp
        void add_input(string, size_t)
        void add_output(string, size_t)
        void add_state(string, string , size_t)


cdef class Sys(CommonSystem):
    cdef CythonSystemImpl * _c_sys
