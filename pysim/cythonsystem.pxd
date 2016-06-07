from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

cdef extern from "CythonSystemImpl.hpp":
    cdef cppclass CythonSystemImpl:
        vector[double*] states
        vector[double*] ders
        void* sysp


cdef class Sys:
    cdef CythonSystemImpl * _c_sys
