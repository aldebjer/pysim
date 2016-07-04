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

cdef class Results:

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = Results()
        p._c_sys = ptr
        p.shp = p._c_sys.getStoreHandlerP()
        return p

    def __dir__(self):
        names = self.shp.getStoreNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return ["time"]+unicodenames

    def __getattr__(self,name):
        cdef np.ndarray[np.double_t,ndim=1, mode='c'] a
        cdef np.ndarray[np.double_t,ndim=2, mode='c'] a_mat
        names = self.shp.getStoreNames()
        unicodenames = [s.decode('utf-8') for s in names]
        if name == "time":
            size = self.shp.getStoreSize()
            a = np.zeros(size, dtype=np.float64) 
            self.shp.fillWithTime(&a[0])
            return a
        elif name in unicodenames:
            bs = bytes(name,'utf-8')
            size = self.shp.getStoreSize()
            columns = self.shp.getStoreColumns(bs)
            a_mat = np.zeros([size,columns],dtype=np.float64)
            self.shp.fillWithStore(bs,&a_mat[0,0],size,columns)
            if columns == 1:
                return a_mat[:,0]
            else:
                return a_mat
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
        scalarnames = self._c_sys.getScalarInputNames()
        scalarnames_uc = [s.decode('utf-8') for s in scalarnames]
        vectornames = self._c_sys.getInputVectorNames()
        vectornames_uc = [s.decode('utf-8') for s in vectornames]
        return scalarnames_uc+vectornames_uc

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getInputVectorNames())
        allscalarnames =  list(self._c_sys.getScalarInputNames())
        if bs in allvectornames:
            return self._c_sys.getInputVector(bs)
        elif bs in allscalarnames:
            return self._c_sys.getScalarInput(bs)
        else:
            raise AttributeError("No input {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getInputVectorNames())
        allscalarnames =  list(self._c_sys.getScalarInputNames())
        if bs in allvectornames:
            try:
                self._c_sys.setInputVector(bs,value)
            except TypeError:
                raise TypeError("Input '{}' is a vector".format(name))
        elif bs in allscalarnames:
            try:
                self._c_sys.setScalarInput(bs,value)
            except TypeError:
                raise TypeError("Input '{}' is a scalar".format(name))
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
        scalarnames = self._c_sys.getScalarOutputNames()
        scalarnames_uc = [s.decode('utf-8') for s in scalarnames]
        vectornames = self._c_sys.getOutputVectorNames()
        vectornames_uc = [s.decode('utf-8') for s in vectornames]
        return scalarnames_uc+vectornames_uc

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getOutputVectorNames())
        allscalarnames =  list(self._c_sys.getScalarOutputNames())
        if bs in allvectornames:
            return self._c_sys.getOutputVector(bs)
        elif bs in allscalarnames:
            return self._c_sys.getScalarOutput(bs)
        else:
            raise AttributeError("No output {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getOutputVectorNames())
        allscalarnames =  list(self._c_sys.getScalarOutputNames())
        if bs in allvectornames:
            try:
                self._c_sys.setOutputVector(bs,value)
            except TypeError:
                raise TypeError("Output '{}' is a vector".format(name))
        elif bs in allscalarnames:
            try:
                self._c_sys.setScalarOutput(bs,value)
            except TypeError:
                raise TypeError("Output '{}' is a scalar".format(name))
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
