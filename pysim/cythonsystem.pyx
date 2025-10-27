"""This module contains functionality for creating a cython or python
system for pysim. Both cython and python systems can inherit from the 
Sys class in this module and then be added to a simulation.
"""

from collections import namedtuple
from libcpp.vector cimport vector
import warnings

import numpy as np
cimport numpy as np

from pysim.cythonsystem cimport CythonSystemImpl
from pysim.commonsystem cimport PysimVars
from pysim.commonsystem cimport Results
from pysim.connections cimport Connections

np.import_array()

cdef class Sys:
    """This class is meant to be inherited by python and cython systems.
    They can then be set up and after that used in a simulation together with
    other systems, both c++, python and cython systems.
    """
    def __cinit__(self):
        cdef CythonSystemImpl* _c_sys_local
        _c_sys_local = new CythonSystemImpl()
        self._c_sys = _c_sys_local
        self._c_s = <CommonSystemImpl*> _c_sys_local
        self._c_sys.sysp = <void*> self
        self._SimulatableSystemInterface_p = _c_sys_local

        self.inputs = PysimVars._create(&_c_sys_local.inputs)
        self.outputs = PysimVars._create(&_c_sys_local.outputs)
        self.states = PysimVars._create(&_c_sys_local.states)
        self.ders = PysimVars._create(&_c_sys_local.ders)
        self.connections = Connections._create(&_c_sys_local.connectionHandler)
        self.res = Results._create(_c_sys_local.getStoreHandlerP())
        self.stores = []

    def __dealloc__(self):
        del self._c_sys

    def add_state(self, statename, dername, rows = 1, cols = 1):
        warnings.warn("depreciated, use add_state_(scalar|vector|matrix) instead",DeprecationWarning)
        statename_bytes = bytes(statename,'utf-8')
        dername_bytes = bytes(dername,'utf-8')
        if cols == 1:
            if rows == 1:
                self._c_sys.add_state_scalar(statename_bytes,dername_bytes, "No Description")
            else:
                self._c_sys.add_state_vector(statename_bytes,dername_bytes, rows, "No Description")
        else:
            self._c_sys.add_state_matrix(statename_bytes,dername_bytes,rows,cols, "No Description")

    def add_state_scalar(self, statename, dername, description = "No Description"):
        statename_bytes = bytes(statename,'utf-8')
        dername_bytes = bytes(dername,'utf-8')
        desc_bytes = bytes(description,'utf-8')
        self._c_sys.add_state_scalar(statename_bytes,dername_bytes,desc_bytes)
       
    def add_state_vector(self, statename, dername, rows, description = "No Description"):
        statename_bytes = bytes(statename,'utf-8')
        dername_bytes = bytes(dername,'utf-8')
        desc_bytes = bytes(description,'utf-8')
        self._c_sys.add_state_vector(statename_bytes,dername_bytes, rows, desc_bytes)

    def add_state_matrix(self, statename, dername, rows, cols, description = "No Description"):
        statename_bytes = bytes(statename,'utf-8')
        dername_bytes = bytes(dername,'utf-8')
        desc_bytes = bytes(description,'utf-8')
        self._c_sys.add_state_matrix(statename_bytes,dername_bytes, rows, cols, desc_bytes)

    def add_input(self, name, rows = 1,cols = 1):
        bs = bytes(name,'utf-8')
        warnings.warn("depreciated, use add_input_(scalar|vector|matrix) instead",DeprecationWarning)
        if cols == 1:
            if rows == 1:
                self._c_sys.add_input_scalar(bs, "No Description")
            else:
                self._c_sys.add_input_vector(bs, rows, "No Description")
        else:
            self._c_sys.add_input_matrix(bs,rows,cols, "No Description")

    def add_input_scalar(self,name, description = "No Description"):
        bs = bytes(name,'utf-8')
        desc_bytes = bytes(description,'utf-8')
        self._c_sys.add_input_scalar(bs,desc_bytes)

    def add_input_vector(self, name, rows, description = "No Description"):
        bs = bytes(name,'utf-8')
        desc_bytes = bytes(description,'utf-8')
        self._c_sys.add_input_vector(bs,rows,desc_bytes)

    def add_input_matrix(self, name, rows, columns, description = "No Description"):
        bs = bytes(name,'utf-8')
        desc_bytes = bytes(description,'utf-8')
        self._c_sys.add_input_matrix(bs,rows, columns,desc_bytes)

    def add_output(self, name, rows = 1, cols = 1):
        warnings.warn("depreciated, use add_output_(scalar|vector|matrix) instead",DeprecationWarning)
        bs = bytes(name,'utf-8')
        if cols == 1:
            if rows == 1:
                self._c_sys.add_output_scalar(bs, b"No Description")
            else:
                self._c_sys.add_output_vector(bs, rows, b"No Description")
        else:
            self._c_sys.add_output_matrix(bs,rows, cols, b"No Description")

    def add_output_scalar(self,name, desc = "No description"):
        utf_name = bytes(name,'utf-8')
        utf_desc = bytes(desc,'utf-8')
        self._c_sys.add_output_scalar(utf_name, utf_desc)

    def add_output_vector(self, name, rows, desc = "No description"):
        utf_name = bytes(name,'utf-8')
        utf_desc = bytes(desc,'utf-8')
        self._c_sys.add_output_vector(utf_name,rows, utf_desc)

    def add_output_matrix(self, name, rows, columns, desc = "No description"):
        utf_name = bytes(name,'utf-8')
        utf_desc = bytes(desc,'utf-8')
        self._c_sys.add_output_matrix(utf_name,rows, columns, utf_desc)

    cpdef void pre_step(self):
        pass

    def do_step(self,time):
        print("stepping {}".format(time))

    cpdef void post_step(self):
        pass


cdef api void pre_step_callback(void* sys):
    cdef Sys s
    s = <Sys> (sys)
    s.pre_step()

cdef api void step_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.do_step(time)

cdef api void post_step_callback(void* sys):
    cdef Sys s
    s = <Sys> (sys)
    s.post_step()

