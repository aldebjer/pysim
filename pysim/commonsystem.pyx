from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from commonsystem cimport CommonSystemImpl

np.import_array()

cdef class CommonSystem:
    def store(self,name):
        bs = bytes(name,'utf-8')
        self._c_s.store(bs)

class Results:

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = Inputs()
        p._c_sys = ptr
        return p

    def __dir__(self):
        return ["time"]+self._c_sys._getStoreNames()

    def __getattr__(self,name):
        if name == "time":
            return self._c_sys._getTime()
        elif name in self._c_sys._getStoreNames():
            return self._c_sys._getStore(name)
        else:
            raise AttributeError("No stored {} in system".format(name))

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

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getOutputVectorNames())
        if bs in allvectornames:
            try:
                self._c_sys.setOutputVector(bs,value)
            except TypeError:
                raise TypeError("Output '{}' is a vector".format(name))
        else:
            raise AttributeError("No output {} in system".format(name))

    def get_description(self,varname):
        return self._c_sys.getOutputDescriptionMap()[varname]

cdef class States:
    """Contains all the states for the system.
    To list the inputs use ouput.list()
    To search for a input that contains a specific word, or has a 
    description that contains the word use input.search(word).
    """

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = States()
        p._c_sys = ptr
        return p

    def __dir__(self):
        vectornames = self._c_sys.getStateVectorNames()
        return vectornames

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getStateVectorNames())
        if bs in allvectornames:
            return self._c_sys.getStateVector(bs)
        else:
            raise AttributeError("No state {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getStateVectorNames())
        if bs in allvectornames:
            try:
                self._c_sys.setStateVector(bs,value)
            except TypeError:
                raise TypeError("State '{}' is a vector".format(name))
        else:
            raise AttributeError("No state {} in system".format(name))

    def get_description(self,varname):
        return self._c_sys.getStateDescriptionMap()[varname]

cdef class Ders:
    """Contains all the Derivates for the system.
    To list the inputs use ouput.list()
    To search for a input that contains a specific word, or has a 
    description that contains the word use input.search(word).
    """

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = Ders()
        p._c_sys = ptr
        return p

    def __dir__(self):
        vectornames = self._c_sys.getDerVectorNames()
        return vectornames

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getDerVectorNames())
        if bs in allvectornames:
            return self._c_sys.getDerVector(bs)
        else:
            raise AttributeError("No der {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getDerVectorNames())
        if bs in allvectornames:
            try:
                self._c_sys.setDerVector(bs,value)
            except TypeError:
                raise TypeError("Der '{}' is a vector".format(name))
        else:
            raise AttributeError("No der {} in system".format(name))

    def get_description(self,varname):
        return self._c_sys.getDerDescriptionMap()[varname]

cdef class Connections:
    """The connections of the system.
    The connections are either from an output or a state of a system.
    """

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = Connections()
        p._c_sys = ptr
        return p

    def connect(self,outputname,CommonSystem inputsys,inputname):
         bsout =  bytes(outputname,'utf-8')
         bsin =  bytes(inputname,'utf-8')
         print("connecting")
         self._c_sys.connect(bsout,inputsys._c_s,bsin)
