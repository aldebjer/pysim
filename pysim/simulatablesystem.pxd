
cdef extern from "SimulatableSystem.hpp" namespace "pysim":
    cdef cppclass SimulatableSystemInterface:
        pass

cdef class SimulatableSystem:
    cdef SimulatableSystemInterface * _SimulatableSystemInterface_p

    
