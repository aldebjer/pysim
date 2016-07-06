import numpy as np
cimport numpy as np

from cppsystem cimport CppSystem

from commonsystem cimport Parameters
from commonsystem cimport Inputs
from commonsystem cimport Outputs
from commonsystem cimport States
from commonsystem cimport Ders
from commonsystem cimport Results
from commonsystem cimport Connections

np.import_array()

cdef class Sys:
    """Represents a System, to be added to an Simulation for evaluation.
       Each System consists of parameters, inputs set prior to the simulation,
       States and State derivatives, and Variables, that may be stored during
       a simulation"""


#    def __dealloc__(self):
#        del self._c_sys

    cdef _setupParVar(self):
        cdef CommonSystemImpl* sp = self._c_s
        self.pars = Parameters._create(sp)
        self.inputs = Inputs._create(sp)
        self.outputs = Outputs._create(sp)
        self.states = States._create(sp)
        self.ders = Ders._create(sp)
        self.connections = Connections._create(sp)
        self.res = Results._create(sp)

    def add_break_greater(self,name,value):
        """Add a break that will be activated if the value of the variable
        or state is larger than the value supplied as argument
        """
        bname = bytes(name,'utf-8')
        self._c_sys.add_compare_greater(bname,value)

    def add_break_smaller(self,name,value):
        """Add a break that will be activated if the value of the variable
        or state is smaller than the value supplied as argument
        """
        bname = bytes(name,'utf-8')
        self._c_sys.add_compare_smaller(bname,value)
