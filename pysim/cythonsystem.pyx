from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from cythonsystem cimport CythonSystemImpl

np.import_array()


cdef class Sys:
    def __cinit__(self):
        self._c_sys = new CythonSystemImpl()
        self._c_sys.sysp = <void*> self
        self.statedict = {}
        self.derdict = {}


    def add_state(self, statename, dername, dimensions):
        cdef np.ndarray[double,mode="c"] state_array =  np.zeros(dimensions)
        self.statedict[statename] = state_array

        cdef np.ndarray[double,mode="c"] der_array =  np.zeros(dimensions)
        self.derdict[dername] = der_array

        cdef int i
        for i in range(dimensions):
            self._c_sys.states.push_back(&state_array[i])
            self._c_sys.ders.push_back(&der_array[i])


    def dostep(self,time):
        print("stepping {}".format(time))


cdef api void step_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.dostep(time)


