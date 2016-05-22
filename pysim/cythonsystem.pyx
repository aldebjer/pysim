from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from cythonsystem cimport CythonSystemImpl

np.import_array()



cdef class Sys:

    def __cinit__(self):
        self._c_sys = new CythonSystemImpl()
        print("created")


