from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from commonsystem cimport CommonSystemImpl
from commonsystem cimport CommonSystem

cdef extern from "CppSystem.hpp":

    cdef cppclass CppSystem(CommonSystemImpl):
        void add_compare_greater(char* comparename, double comparevalue) except +
        void add_compare_smaller(char* comparename, double comparevalue) except +
        bool do_comparison() except +

cdef class Sys(CommonSystem):
    cdef CppSystem * _c_sys
    cdef _setupParVar(self)
