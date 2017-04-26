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
        self.pars = Par()

    def __dealloc__(self):
        del self._c_sys
        
    def __iter__(self):
        """Iterating over subsystems
        """
        return iter(self.subsystems.values())
        
    def store(self,name):
        """Store a input or output in the composite system if existing and
        an input, output, state or der in all subsystems if existing.

        Parameters
        ----------
        name : str
            The first parameter.
        """
        vars = dir(self.inputs) + dir(self.outputs)
        if name in vars:
            bs = bytes(name,'utf-8')
            self._c_sys.store(bs)

        for sys in self:
            vars = dir(sys.inputs) + dir(sys.states) + dir(sys.ders) + dir(sys.outputs)
            if name in vars:
                sys.store(name)
                
    def store_all(self, sys=None):
        '''Method for storing all inputs, states,
        ders and outputs of the composite system
        and its subsystems recursively.
        '''
        if not sys:
            self.store_all(sys = self)
            return

        for input in dir(sys.inputs):
            sys.store(input)

        for output in dir(sys.outputs):
            sys.store(output)

        if isinstance(sys, CompositeSystem):
            for subsystem in sys.subsystems.values():
                subsystem.store_all()
                
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
        
    def expand_single_input(self, portname, sys, sysport, initialvalue):
        if type(initialvalue)==list:
            self.add_port_in_vector(portname, initialvalue, '')
            self.connect_port_in(portname, sys, sysport)
        elif type(initialvalue)==int or type(initialvalue)==float:
            self.add_port_in_scalar(portname, initialvalue, '')
            self.connect_port_in(portname, sys, sysport)
        else:
            warning_text = 'Couldnt expand input port with portname {}'.format(portname)
            raise ConnectionAbortedError(warning_text)

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
        
    def expand_single_output(self, portname, sys, sysport, initialvalue):
        '''Method for expanding single output port'''
        if type(initialvalue)==list:
            self.add_port_out_vector(portname, initialvalue, '')
            self.connect_port_out(portname, sys, sysport)
        elif type(initialvalue)==int or type(initialvalue)==float:
            self.add_port_out_scalar(portname, initialvalue, '')
            self.connect_port_out(portname, sys, sysport)
        else:
            warning_text = 'Couldnt expand output port with portname {}'.format(portname)
            raise ConnectionAbortedError(warning_text)
        
    #Help functions for setting up a composite system
    def expand_single_port(self, name, system, initialvalue):
        if name in dir(system.inputs):
            self.expand_single_input(name, system, name, initialvalue)
        else:
            self.expand_single_output(name, system, name, initialvalue)

class Par():
    '''Help class for emulating pars of composite systems
    '''
    def __init__(self):
        self.__dict__['parameters'] = {}

    def add_par(self, sys, variable):
        '''Method for adding a par to be emulated
        '''
        self.parameters[variable] = sys

    def __getattr__(self, variable):
        '''Overriding __getattr__
        '''
        assert variable in self.parameters.keys()
        getattr(self.parameters.get(variable).pars, variable)

    def __setattr__(self, variable, value):
        '''Overriding __setattr__
        '''
        assert variable in self.parameters.keys()
        setattr(self.parameters.get(variable).pars, variable, value)