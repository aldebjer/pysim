from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from simulatablesystem cimport SimulatableSystem
from commonsystem cimport CommonSystemImpl
from commonsystem cimport CommonSystem
from commonsystem cimport Variable
from commonsystem cimport PysimVars

cdef extern from "CompositeSystemImpl.hpp" namespace "pysim":
    cdef cppclass CompositeSystemImpl(SimulatableSystem):

        Variable inputs
        Variable outputs

        void connect(char*, CommonSystemImpl*, char* )
        void add_subsystem(CommonSystemImpl* , string)
        void add_input_port(string, string, string, string)
        void add_output_port(string, string, string, string)


cdef class CompositeSystem:
    cdef CompositeSystemImpl * _c_sys
    cdef public PysimVars inputs
