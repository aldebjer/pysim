from collections import namedtuple
from libcpp.vector cimport vector
import warnings

import numpy as np
cimport numpy as np
from cythonsystem cimport CythonSystemImpl
from commonsystem cimport PysimVars
from commonsystem cimport Results
#cimport simulatablesystem

from connections cimport Connections

np.import_array()

cdef class Sys:

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

    def __dealloc__(self):
        del self._c_sys

    def add_state(self, statename, dername, rows = 1, cols = 1):
        statename_bytes = bytes(statename,'utf-8')
        dername_bytes = bytes(dername,'utf-8')
        if cols == 1:
            self._c_sys.add_state(statename_bytes,dername_bytes,rows)
        else:
            self._c_sys.add_state(statename_bytes,dername_bytes,rows,cols)

    def add_input(self, name, rows = 1,cols = 1):
        bs = bytes(name,'utf-8')
        warnings.warn("depreciated, use add_input_(scalar|vector|matrix) instead",DeprecationWarning)
        if cols == 1:
            self._c_sys.add_input(bs,rows)
        else:
            self._c_sys.add_input_matrix(bs,rows,cols)

    def add_input_scalar(self,name):
        bs = bytes(name,'utf-8')
        self._c_sys.add_input_scalar(bs)

    def add_input_vector(self, name, rows):
        bs = bytes(name,'utf-8')
        self._c_sys.add_input_vector(bs,rows)

    def add_input_matrix(self, name, rows, columns):
        bs = bytes(name,'utf-8')
        self._c_sys.add_input_matrix(bs,rows, columns)

    def add_output(self, name, rows = 1, cols = 1):
        warnings.warn("depreciated, use add_output_(scalar|vector|matrix) instead",DeprecationWarning)
        bs = bytes(name,'utf-8')
        if cols == 1:
            self._c_sys.add_output(bs,rows)
        else:
            self._c_sys.add_output_matrix(bs,rows, cols, "No Description")

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

    def do_step(self,time):
        print("stepping {}".format(time))


cdef api void step_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.do_step(time)

