from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from commonsystem cimport CommonSystemImpl
from commonsystem cimport CommonSystem

cdef extern from "CythonSystemImpl.hpp":
    cdef cppclass CythonSystemImpl(CommonSystemImpl):
        vector[double*] states
        vector[double*] ders
        void* sysp
        void add_input_vector(string, size_t)
        void add_output_vector(string, size_t)
        void add_state_vector(string, string , size_t)


cdef class Sys(CommonSystem):
    cdef CythonSystemImpl * _c_sys
