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
        void add_input_scalar(string, string)
        void add_input_vector(string, size_t, string)
        void add_input_matrix(string, size_t, size_t, string)
        void add_output(string, size_t)
        void add_output_scalar(string, string)
        void add_output_vector(string, size_t, string)
        void add_output_matrix(string, size_t, size_t, string)
        void add_state(string, string , size_t)
        void add_state_scalar(string, string, string)
        void add_state_vector(string, string, size_t, string)
        void add_state_matrix(string, string, size_t, size_t, string)


cdef class Sys(CommonSystem):
    cdef CythonSystemImpl * _c_sys
    cpdef void pre_step(Sys)
    cpdef void post_step(Sys)
