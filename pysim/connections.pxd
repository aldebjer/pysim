from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from commonsystem cimport CommonSystemImpl


cdef extern from "ConnectionHandler.hpp" namespace "pysim":
    cdef cppclass ConnectionHandler:
        void connect(char*, CommonSystemImpl*, char*) except +


cdef class Connections:
    cdef ConnectionHandler* _c_connectionHandler
    cdef readonly object connection_list
    @staticmethod
    cdef _create(ConnectionHandler* ptr)
