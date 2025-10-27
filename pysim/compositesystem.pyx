import warnings
from collections import namedtuple, OrderedDict
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np

from pysim.compositesystem cimport CompositeSystemImpl
from pysim.compositesystem cimport CompositeSystem
from pysim.commonsystem cimport Results
from pysim.commonsystem cimport CommonSystem
from pysim.simulatablesystem cimport SimulatableSystem

np.import_array()

ctypedef fused NestableSystem:
    CompositeSystem
    CommonSystem

cdef class CompositeSystem(SimulatableSystem):

    def __cinit__(self):
        cdef CompositeSystemImpl* _c_sys_local
        _c_sys_local = new CompositeSystemImpl()
        self._c_sys = _c_sys_local
        self._SimulatableSystemInterface_p = _c_sys_local

        self.inputs = PysimVars._create(&_c_sys_local.inputs)
        self.outputs = PysimVars._create(&_c_sys_local.outputs)
        self.connections = Connections._create(&_c_sys_local.connectionHandler)
        self.subsystems = OrderedDict()
        self.res = Results._create(_c_sys_local.getStoreHandlerP())
        self.pars = Par()
        self.stores = []
        self.out_ports = {}
        self.in_ports = {}

    def __dealloc__(self):
        del self._c_sys

    def store(self,name):
        """Store a input or output in the system.

        Parameters
        ----------
        name : str
            Name of the variable to store.
        """
        bs = bytes(name,'utf-8')
        self._c_sys.store(bs)
        self.stores.append(name)

    def set_store_interval(self, interval):
        """Set the store interval of this system. 
        By default the
        store interval for all systems in a simulation is set by
        the simulation, typically for the step length for fixed
        step algorithms. By calling this function the store interval
        *for this system* is set differently.
        Parameters
        ----------
        interval : float
            The time between stores, in seconds.
        """
        self._c_sys.getStoreHandlerP().setStoreInterval(interval)

        for subsystem in self.subsystems.values():
            subsystem.set_store_interval(interval)

    def store_recursively(self,name):
        """Store a input or output in the composite system if existing and
        an input, output, state or der in all subsystems if existing.

        Parameters
        ----------
        name : str
            Name of the variable to store.
        """
        self.store(self, name)

        for subsystem in self.subsystems.values():
            if type(subsystem) is CompositeSystem:
                subsystem.store_recursive()
            else:
                subsystem.store()

    def store_all(self):
        """Method for storing all inputs, states,
        ders and outputs of the composite system.
        """

        for input in dir(self.inputs):
            self.store(input)

        for output in dir(self.outputs):
            self.store(output)

    def store_all_recursively(self):
        '''Method for storing all inputs, states,
        ders and outputs of the composite system
        and its subsystems recursively.
        '''
        self.store_all()

        for subsystem in self.subsystems.values():
            if type(subsystem) is CompositeSystem:
                subsystem.store_all_recursively()
            else:
                subsystem.store_all()

    def add_subsystem(self, NestableSystem subsystem, name):
        self.subsystems[name] = subsystem
        bs = bytes(name,'utf-8')
        if NestableSystem is CommonSystem:
            self._c_sys.add_subsystem(<CommonSystemImpl*>subsystem._c_s, bs)
        elif NestableSystem is CompositeSystem:
            self._c_sys.add_composite_subsystem(<CompositeSystemImpl*>subsystem._c_sys, bs)

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
        self.in_ports[name] = {"type": "scalar",
                                "value":initialvalue,
                                "description": description,
                                "connections": []
                               }

    def add_port_in_vector(self, name, initialvalue, description):
        bs_names =  bytes(name,'utf-8')
        bs_desc = bytes(description,'utf-8')
        self._c_sys.add_vector_inport(bs_names, initialvalue, bs_desc)
        self.in_ports[name] = {"type": "vector",
                                "value":initialvalue,
                                "description": description,
                                "connections": []
                               }

    def add_port_in_matrix(self, name, initialvalue, description):
        bs_names =  bytes(name,'utf-8')
        bs_desc = bytes(description,'utf-8')
        self._c_sys.add_matrix_inport(bs_names, initialvalue, bs_desc)
        self.in_ports[name] = {"type": "matrix",
                                "value":initialvalue,
                                "description": description,
                                "connections": []
                               }
        
    def expand_single_input(self, portname, sys, sysport, initialvalue):
        ''' Method for expanding an input port, i.e. setting an input port 
        of a subsystem as and input port to the Compositesystem and 
        connecting them.
        '''
        if type(initialvalue)==list:
            self.add_port_in_vector(portname, initialvalue, '')
            self.connect_port_in(portname, sys, sysport)
        elif type(initialvalue)==int or type(initialvalue)==float:
            self.add_port_in_scalar(portname, initialvalue, '')
            self.connect_port_in(portname, sys, sysport)
        else:
            warning_text = 'Couldnt expand input port with portname {}'.format(portname)
            raise ValueError(warning_text)

    # Add Output ports
    def add_port_out_scalar(self, name, initialvalue, description):
        cdef string bs_names =  bytes(name,'utf-8')
        cdef string bs_desc = bytes(description,'utf-8')
        cdef double d_initial = initialvalue
        self._c_sys.add_outport(bs_names, d_initial, bs_desc)
        self.out_ports[name] = {"type": "scalar",
                                "value":initialvalue,
                                "description": description,
                                "connections": []
                               }

    def add_port_out_vector(self, name, initialvalue, description):
        cdef string bs_names =  bytes(name,'utf-8')
        cdef string bs_desc = bytes(description,'utf-8')
        cdef vector[double] v_initial = initialvalue
        self._c_sys.add_outport(bs_names, v_initial, bs_desc)
        self.out_ports[name] = {"type": "vector",
                                "value":initialvalue,
                                "description": description,
                                "connections": []
                               }

    def add_port_out_matrix(self, name, initialvalue, description):
        cdef string bs_names =  bytes(name,'utf-8')
        cdef string bs_desc = bytes(description,'utf-8')
        cdef vector[vector[double]] m_initial = initialvalue
        self._c_sys.add_outport(bs_names, m_initial, bs_desc)
        self.out_ports[name] = {"type": "matrix",
                                "value":initialvalue,
                                "description": description,
                                "connections": []
                               }

    def _get_subsystem_name(self, NestableSystem system):
        for name,subsys in self.subsystems.items():
            if id(subsys) == id(system):
                return name
        raise ValueError('argument is not a system')

    def connect_port_in(self, portname, NestableSystem receiving_system, inputname):
        bs_portname =  bytes(portname,'utf-8')
        bs_inputname = bytes(inputname,'utf-8')
        if NestableSystem is CommonSystem:
            self._c_sys.connect_port_in(bs_portname, receiving_system._c_s, bs_inputname)
        elif NestableSystem is CompositeSystem:
            self._c_sys.connect_port_in_composite(bs_portname, receiving_system._c_sys, bs_inputname)
        con = {"subsystem": self._get_subsystem_name(receiving_system),
               "input": inputname,
              }
        self.in_ports[portname]["connections"].append(con)

    def connect_port_out(self, portname, NestableSystem output_system, output_name):
        bs_portname =  bytes(portname,'utf-8')
        bs_output_name = bytes(output_name,'utf-8')
        if NestableSystem is CommonSystem:
            self._c_sys.connect_port_out(bs_portname, output_system._c_s, bs_output_name)
        elif NestableSystem is CompositeSystem:
            self._c_sys.connect_port_out_composite(bs_portname, output_system._c_sys, bs_output_name)
        con = {"subsystem": self._get_subsystem_name(output_system),
               "output": output_name,
              }
        self.out_ports[portname]["connections"].append(con)

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
            raise ValueError(warning_text)

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
