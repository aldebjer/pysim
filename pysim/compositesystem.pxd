from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from simulatablesystem cimport SimulatableSystemInterface
from simulatablesystem cimport SimulatableSystem

from commonsystem cimport CommonSystemImpl
from compositesystem cimport CommonSystemImpl


from commonsystem cimport Variable
from commonsystem cimport PysimVars
from commonsystem cimport StoreHandler
from connections cimport Connections
from connections cimport ConnectionHandler

cdef extern from "CompositeSystemImpl.hpp" namespace "pysim":
    cdef cppclass CompositeSystemImpl(SimulatableSystemInterface):

        Variable inputs
        Variable outputs
        ConnectionHandler connectionHandler

        #void connect(char*, CommonSystemImpl*, char* )
        void add_subsystem(CommonSystemImpl* , string) except +
        void add_composite_subsystem(CompositeSystemImpl*, string) except +

        void add_scalar_port_in(string name, double initial_value, string description) except +
        void add_vector_inport(string name, vector[double] initial_value, string description) except +
        void add_matrix_inport(string name, vector[vector[double]] initial_value, string description) except +

        void add_outport(string name, double initial_value, string description) except +
        void add_outport(string name, vector[double] initial_value, string description) except +
        void add_outport(string name, vector[vector[double]] initial_value, string description) except +

        void connect_port_in(string portname, CommonSystemImpl* subsystem, string subsystem_input) except +
        void connect_port_in_composite(string portname, CompositeSystemImpl* subsystem, string subsystem_input) except +
        void connect_port_out(string portname, CommonSystemImpl* subsystem, string subsystem_output) except +
        void connect_port_out_composite(string portname, CompositeSystemImpl* subsystem, string subsystem_output) except +

        void store(char* name) except +
        StoreHandler* getStoreHandlerP()


cdef class CompositeSystem(SimulatableSystem):
    cdef CompositeSystemImpl * _c_sys
    cdef public PysimVars inputs
    cdef public PysimVars outputs
    cdef public object subsystems
    cdef public object res
    cdef public object pars
    cdef public object stores
    cdef public object out_ports
    cdef public object in_ports
    cdef public Connections connections

