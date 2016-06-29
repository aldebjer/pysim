from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from cythonsystem cimport CythonSystemImpl
from commonsystem cimport Inputs
from commonsystem cimport Outputs
from commonsystem cimport Connections

np.import_array()

class Results:
    def __init__(self,storedict):
        self.storedict = storedict
    def __dir__(self):
        return list(self.storedict.keys())
    def __getattr__(self,name):
        return np.array(self.storedict[name])

class States:
    def __init__(self,statedict):
        self.__dict__["_statedict"] = statedict
    def __dir__(self):
        return list(self._statedict.keys())
    def __getattr__(self,name):
        return self._statedict[name]
    def __setattr__(self,name,value):
        np.copyto(self._statedict[name], value)

cdef class Sys:

    def __cinit__(self):
        cdef CythonSystemImpl* _c_sys_local
        _c_sys_local = new CythonSystemImpl()
        self._c_sys = _c_sys_local
        self._c_s = <CommonSystemImpl*> _c_sys_local
        self._c_sys.sysp = <void*> self
        self._statedict = {}
        self._derdict = {}
        self.storedict = {}
        self.res = Results(self.storedict)
        self.states = States(self._statedict)
        self.inputs = Inputs._create(_c_sys_local)
        self.outputs = Outputs._create(_c_sys_local)
        self.connections = Connections._create(_c_sys_local)


    def add_state(self, statename, dername, dimensions):
        cdef np.ndarray[double,mode="c"] state_array =  np.zeros(dimensions)
        self._statedict[statename] = state_array

        cdef np.ndarray[double,mode="c"] der_array =  np.zeros(dimensions)
        self._derdict[dername] = der_array

        cdef int i
        for i in range(dimensions):
            self._c_sys.states.push_back(&state_array[i])
            self._c_sys.ders.push_back(&der_array[i])

    def add_input(self, name, size):
        bs = bytes(name,'utf-8')
        self._c_sys.add_input_vector(bs,size)


    def add_output(self, name, size):
        bs = bytes(name,'utf-8')
        self._c_sys.add_output_vector(bs,size)

    def store(self,name):
        self.storedict[name] = []


    def do_step(self,time):
        print("stepping {}".format(time))

    def do_storestep(self,time):
        for name in self.storedict:
            self.storedict[name].append(self._statedict[name].copy())


cdef api void step_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.do_step(time)

cdef api void storestep_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.do_storestep(time)


