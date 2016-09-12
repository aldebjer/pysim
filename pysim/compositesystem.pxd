from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

from simulatablesystem cimport SimulatableSystemInterface
from simulatablesystem cimport SimulatableSystem

from commonsystem cimport CommonSystemImpl

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
        void add_input_port(string, string, string, string) except +
        void add_output_port(string, string, string, string) except +

        void store(char* name)
        StoreHandler* getStoreHandlerP()


cdef class CompositeSystem(SimulatableSystem):
    cdef CompositeSystemImpl * _c_sys
    cdef public PysimVars inputs
    cdef public Connections connections
