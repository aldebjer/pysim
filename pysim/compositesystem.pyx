import warnings
from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np

from compositesystem cimport CompositeSystemImpl
from commonsystem cimport Results
from commonsystem cimport CommonSystem
from simulatablesystem cimport SimulatableSystem

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
        self.subsystems = {}
        self.res = Results._create(_c_sys_local.getStoreHandlerP())

    def __dealloc__(self):
        del self._c_sys

    def store(self,name):
        """Store a input or output in the system.

        Parameters
        ----------
        name : str
            The first parameter.
        """

        bs = bytes(name,'utf-8')
        self._c_sys.store(bs)

    def add_subsystem(self, CommonSystem subsystem, name):
        self.subsystems[name] = subsystem
        bs = bytes(name,'utf-8')
        self._c_sys.add_subsystem(subsystem._c_s, bs)

    def add_input_port(self, name, subsystemname, subsystem_input, description):
        warnings.warn("Deprecated, use add_port_in_scalar-connect_port_in instead", DeprecationWarning)
        cdef CommonSystem sys = self.subsystems[subsystemname]
        self.add_port_in_scalar(name, 0.0, description)
        self.connect_port_in(name, sys, subsystem_input)

    def add_output_port(self, name, subsystemname, subsystem_output, description):
        warnings.warn("Deprecated, use add_port_out_scalar-connect_port_out instead", DeprecationWarning)
        cdef CommonSystem sys = self.subsystems[subsystemname]
        self.add_port_out_scalar(name, 0.0, description)
        self.connect_port_out(name, sys, subsystem_output)

    #Add Input Ports
    def add_port_in_scalar(self, name, initialvalue, description):
        bs_names =  bytes(name,'utf-8')
        bs_desc = bytes(description,'utf-8')
        self._c_sys.add_scalar_port_in(bs_names, initialvalue, bs_desc)

    def add_port_in_vector(self, name, initialvalue, description):
        bs_names =  bytes(name,'utf-8')
        bs_desc = bytes(description,'utf-8')
        self._c_sys.add_vector_inport(bs_names, initialvalue, bs_desc)

    def add_port_in_matrix(self, name, initialvalue, description):
        bs_names =  bytes(name,'utf-8')
        bs_desc = bytes(description,'utf-8')
        self._c_sys.add_matrix_inport(bs_names, initialvalue, bs_desc)

    # Add Output ports
    def add_port_out_scalar(self, name, initialvalue, description):
        cdef string bs_names =  bytes(name,'utf-8')
        cdef string bs_desc = bytes(description,'utf-8')
        cdef double d_initial = initialvalue
        self._c_sys.add_outport(bs_names, d_initial, bs_desc)

    def add_port_out_vector(self, name, initialvalue, description):
        cdef string bs_names =  bytes(name,'utf-8')
        cdef string bs_desc = bytes(description,'utf-8')
        cdef vector[double] v_initial = initialvalue
        self._c_sys.add_outport(bs_names, v_initial, bs_desc)

    def add_port_out_matrix(self, name, initialvalue, description):
        cdef string bs_names =  bytes(name,'utf-8')
        cdef string bs_desc = bytes(description,'utf-8')
        cdef vector[vector[double]] m_initial = initialvalue
        self._c_sys.add_outport(bs_names, m_initial, bs_desc)

    def connect_port_in(self, portname, CommonSystem receiving_system, inputname):
        bs_portname =  bytes(portname,'utf-8')
        bs_inputname = bytes(inputname,'utf-8')
        self._c_sys.connect_port_in(bs_portname, receiving_system._c_s, bs_inputname)

    def connect_port_out(self, portname, CommonSystem output_system, output_name):
        bs_portname =  bytes(portname,'utf-8')
        bs_output_name = bytes(output_name,'utf-8')
        self._c_sys.connect_port_out(bs_portname, output_system._c_s, bs_output_name)
