from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map


cdef extern from "ConnectionHandler.hpp" namespace "pysim":
    cdef cppclass ConnectionHandler:
        void connect[T](char*, T*, char*) except +
        void connect[T](char*, T*, char*, int) except +


cdef class Connections:
    cdef ConnectionHandler* _c_connectionHandler
    @staticmethod
    cdef _create(ConnectionHandler* ptr)
