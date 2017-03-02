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


    def __dealloc__(self):
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
