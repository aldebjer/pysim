from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from cythonsystem cimport CythonSystemImpl

np.import_array()

class Results:
    def __init__(self,storedict):
        self.storedict = storedict
    def __dir__(self):
        list(self.storedict.keys())
    def __getattr__(self,name):
        return np.array(self.storedict[name])


cdef class Sys:
    def __cinit__(self):
        self._c_sys = new CythonSystemImpl()
        self._c_sys.sysp = <void*> self
        self.statedict = {}
        self.derdict = {}
        self.storedict = {}
        self.res = Results(self.storedict)


    def add_state(self, statename, dername, dimensions):
        cdef np.ndarray[double,mode="c"] state_array =  np.zeros(dimensions)
        self.statedict[statename] = state_array

        cdef np.ndarray[double,mode="c"] der_array =  np.zeros(dimensions)
        self.derdict[dername] = der_array

        cdef int i
        for i in range(dimensions):
            self._c_sys.states.push_back(&state_array[i])
            self._c_sys.ders.push_back(&der_array[i])

    def store(self,name):
        self.storedict[name] = []


    def do_step(self,time):
        print("stepping {}".format(time))

    def do_storestep(self,time):
        for name in self.storedict:
            self.storedict[name].append(self.statedict[name].copy())


cdef api void step_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.do_step(time)

cdef api void storestep_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.do_storestep(time)


