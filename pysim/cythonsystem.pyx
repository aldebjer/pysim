from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from cythonsystem cimport CythonSystemImpl

np.import_array()



cdef class Sys:
    def __cinit__(self):
        self.sp = 5.0
        self.dp = 0.0
        self._c_sys = new CythonSystemImpl()
        self._c_sys.sysp = <void*> self
        print("created")
        self._c_sys.states.push_back(&self.sp)
        self._c_sys.ders.push_back(&self.dp)

    def dostep(self,time):
        print("stepping {}".format(time))


cdef api void step_callback(void* sys, double time):
    cdef Sys s
    s = <Sys> (sys)
    s.dostep(time)


