from simulatablesystem cimport SimulatableSystemInterface


cdef class SimulatableSystem:
    def __cinit__(self):
        self._SimulatableSystemInterface_p = NULL
