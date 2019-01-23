""" Connections between systems
"""

from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np

from commonsystem cimport CommonSystemImpl
from commonsystem cimport CommonSystem

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
        CommonSystem, e.g. CppSystems and CythonSystems.

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
        bsout =  bytes(outputname,'utf-8')
        bsin =  bytes(inputname,'utf-8')

        self._c_connectionHandler.connect(bsout,(<CommonSystem>inputsys)._c_s,bsin)
        self.connection_list.append((outputname, inputsys, inputname))
