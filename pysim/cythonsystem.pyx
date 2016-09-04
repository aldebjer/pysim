from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from cythonsystem cimport CythonSystemImpl
from commonsystem cimport PysimVars
from commonsystem cimport Results
#cimport simulatablesystem

from commonsystem cimport Connections

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
        self.connections = Connections._create(_c_sys_local)
        self.res = Results._create(_c_sys_local.getStoreHandlerP())

    def __dealloc__(self):
        del self._c_sys

    def add_state(self, statename, dername, size = 1):
        statename_bytes = bytes(statename,'utf-8')
        dername_bytes = bytes(dername,'utf-8')
        self._c_sys.add_state(statename_bytes,dername_bytes,size)

    def add_input(self, name, size = 1):
        bs = bytes(name,'utf-8')
        self._c_sys.add_input(bs,size)

    def add_output(self, name, size = 1):
        bs = bytes(name,'utf-8')
        self._c_sys.add_output(bs,size)

    def do_step(self,time):
        print("stepping {}".format(time))


cdef api void step_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.do_step(time)

