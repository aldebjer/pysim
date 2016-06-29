from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from commonsystem cimport CommonSystemImpl

np.import_array()

cdef class Inputs:
    """Contains all the inputs for the system.
    To list the inputs use input.list()
    To search for a input that contains a specific word, or has a 
    description that contains the word use input.search(word).
    """

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = Inputs()
        p._c_sys = ptr
        return p

    def __dir__(self):
        vectornames = self._c_sys.getInputVectorNames()
        return vectornames
    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getInputVectorNames())
        if bs in allvectornames:
            return self._c_sys.getInputVector(bs)
        else:
            raise AttributeError("No input {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getInputVectorNames())
        if bs in allvectornames:
            try:
                self._c_sys.setInputVector(bs,value)
            except TypeError:
                raise TypeError("Input '{}' is a vector".format(name))
        else:
            raise AttributeError("No input {} in system".format(name))

    def get_description(self,varname):
        return self._c_sys.getInputDescriptionMap()[varname]

cdef class Outputs:
    """Contains all the outputs for the system.
    To list the inputs use ouput.list()
    To search for a input that contains a specific word, or has a 
    description that contains the word use input.search(word).
    """

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = Outputs()
        p._c_sys = ptr
        return p

    def __dir__(self):
        vectornames = self._c_sys.getOutputVectorNames()
        return vectornames

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getOutputVectorNames())
        if bs in allvectornames:
            return self._c_sys.getOutputVector(bs)
        else:
            raise AttributeError("No output {} in system".format(name))

    def get_description(self,varname):
        return self._c_sys.getOutputDescriptionMap()[varname]


