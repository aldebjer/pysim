""" Connections between systems
"""

from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np

from commonsystem cimport CommonSystemImpl
from commonsystem cimport CommonSystem
from compositesystem cimport CompositeSystem
from compositesystem cimport CompositeSystemImpl

np.import_array()

cdef class Connections:
    """The connections of the system.
    The connections are either from an output or a state of a system.
    """

    @staticmethod 
    cdef _create(ConnectionHandler* ptr):
        p = Connections()
        p._c_connectionHandler = ptr
        return p

    def add_connection(self,outputname, inputsys,inputname, output_element = None):
        """Connect the outputs from this system to the inputs of another.

        The systems that are to be connected must be derived from a
        CommonSystem, which is true for both CppSystems and CythonSystems.

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

        if isinstance(inputsys,CommonSystem):
            if output_element:
                self._c_connectionHandler.connect[CommonSystemImpl](bsout,(<CommonSystem>inputsys)._c_s,bsin, output_element)
            else:
                self._c_connectionHandler.connect[CommonSystemImpl](bsout,(<CommonSystem>inputsys)._c_s,bsin)
        elif isinstance(inputsys, CompositeSystem):
            if output_element:
                self._c_connectionHandler.connect[CompositeSystemImpl](bsout,(<CompositeSystem>inputsys)._c_sys,bsin, output_element)
            else:
                self._c_connectionHandler.connect[CompositeSystemImpl](bsout,(<CompositeSystem>inputsys)._c_sys,bsin)
