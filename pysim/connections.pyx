""" Connections between systems
"""

from collections import namedtuple
from libcpp.vector cimport vector

from cpython.bytes cimport PyBytes_AsString

import numpy as np
cimport numpy as np

from pysim.commonsystem cimport CommonSystemImpl
from pysim.commonsystem cimport CommonSystem
from pysim.compositesystem cimport CompositeSystem
from pysim.compositesystem cimport CompositeSystemImpl

np.import_array()

cdef class Connections:
    """The connections of the system.
    The connections are either from an output or a state of a system. To add
    a new connection use the function add_connection.
    """

    @staticmethod 
    cdef _create(ConnectionHandler* ptr):
        p = Connections()
        p._c_connectionHandler = ptr
        return p

    def __init__(self):
        self.connection_list = []

    def add_connection(self,outputname, inputsys,inputname, output_element = None):
        """Connect the outputs from this system to the inputs of another.

        The systems that are to be connected must be derived from a
        CommonSystem, e.g. CppSystems and CythonSystems, or be a 
        CompositeSystem.

        Parameters
        ----------
        outputname : str
            The name of the output of this system
        inputsys : CommonSystem
            The system that is to be connected, that will receive the signals.
        inputname : str
            The name of the input that will receive the signal 
        output_index (optional): int
            The index of a vector to use if connecting to a scalar

        Raises
        ------
        ValueError
            If inputname or outputname are not inputs, outputs members
            of the system.

        """
        
        bsout_t =  outputname.encode('utf-8')
        cdef char* bsout = bsout_t

        bsin_t =  inputname.encode('utf-8')
        cdef char* bsin = bsin_t

        cdef int output_element_int

        if isinstance(inputsys,CommonSystem):
            if output_element:
                output_element_int = int(output_element)
                self._c_connectionHandler.connect[CommonSystemImpl](bsout,(<CommonSystem>inputsys)._c_s, bsin, output_element_int)
            else:
                self._c_connectionHandler.connect[CommonSystemImpl](bsout,(<CommonSystem>inputsys)._c_s,bsin)
        elif isinstance(inputsys, CompositeSystem):
            if output_element:
                output_element_int = int(output_element)
                self._c_connectionHandler.connect[CompositeSystemImpl](bsout,(<CompositeSystem>inputsys)._c_sys,bsin, output_element_int)
            else:
                self._c_connectionHandler.connect[CompositeSystemImpl](bsout,(<CompositeSystem>inputsys)._c_sys,bsin)

        self.connection_list.append((outputname, inputsys, inputname))
