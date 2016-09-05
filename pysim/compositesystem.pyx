from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np

from compositesystem cimport CompositeSystemImpl
from commonsystem cimport PysimVars
from commonsystem cimport Results
from commonsystem cimport CommonSystem
from simulatablesystem cimport SimulatableSystem

from commonsystem cimport Connections

np.import_array()

cdef class CompositeSystem(SimulatableSystem):

    def __cinit__(self):
        cdef CompositeSystemImpl* _c_sys_local
        _c_sys_local = new CompositeSystemImpl()
        self._c_sys = _c_sys_local
        self._SimulatableSystemInterface_p = _c_sys_local

        self.inputs = PysimVars._create(&_c_sys_local.inputs)
        self.outputs = PysimVars._create(&_c_sys_local.outputs)
        self.connections = Connections._create(&_c_sys_local.connectionHandler)
        self.res = Results._create(_c_sys_local.getStoreHandlerP())

    def __dealloc__(self):
        del self._c_sys

    def store(self,name):
        """Store a input, output or state in the system.

        Parameters
        ----------
        name : str
            The first parameter.
        """

        bs = bytes(name,'utf-8')
        self._c_sys.store(bs)

    def add_subsystem(self, CommonSystem subsystem, name):
        bs = bytes(name,'utf-8')
        self._c_sys.add_subsystem(subsystem._c_s, bs)

    def add_input_port(self, name, subsystemname, subsystem_input, description):
        string_args = [name, subsystemname,subsystem_input,description]
        bs_args = [bytes(s,'utf-8') for s in string_args]
        self._c_sys.add_input_port(bs_args[0],bs_args[1],bs_args[2],bs_args[3])

    def add_output_port(self, name, subsystemname, subsystem_output, description):
        string_args = [name, subsystemname,subsystem_output,description]
        bs_args = [bytes(s,'utf-8') for s in string_args]
        self._c_sys.add_output_port(bs_args[0],bs_args[1],bs_args[2],bs_args[3])

