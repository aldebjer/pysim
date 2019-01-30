import numpy as np
cimport numpy as np

from cppsystem cimport CppSystem

from commonsystem cimport Parameters
from commonsystem cimport PysimVars
from commonsystem cimport Results
from connections cimport Connections

np.import_array()

cdef class Sys:
    """Represents a System, to be added to an Simulation for evaluation.
       Each System consists of parameters, inputs set prior to the simulation,
       States and State derivatives, and Variables, that may be stored during
       a simulation"""

    def __cinit__(self):
        self._owner = True

    @staticmethod
    cdef Sys from_ptr(CppSystem* sys_p, string name, owner=True):
        cdef Sys s = type(name.decode("utf-8"), (Sys,), {})()
        s._owner = owner
        s._c_s = sys_p
        s._c_sys = sys_p
        s._setupParVar()
        return s

    def __dealloc__(self):
        if self._owner:
            del self._c_sys

    cdef _setupParVar(self):
        cdef CommonSystemImpl* sp = self._c_s
        self._SimulatableSystemInterface_p = sp
        self.pars = Parameters._create(sp)
        self.inputs = PysimVars._create(&sp.inputs)
        self.outputs = PysimVars._create(&sp.outputs)
        self.states = PysimVars._create(&sp.states)
        self.ders = PysimVars._create(&sp.ders)
        self.connections = Connections._create(&sp.connectionHandler)
        self.res = Results._create(sp.getStoreHandlerP())
        self.stores = []
        self._get_subsystems()

    cdef _get_subsystems(self):
        cdef CppSystem* c_p

        for name in self._c_s.subsystem_names:
            c_p = <CppSystem*>self._c_s.get_subsystem(name)
            s = Sys.from_ptr(c_p, name, False)
            self._subsystems[name.decode("utf-8")] = s