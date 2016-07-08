from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from commonsystem cimport CommonSystemImpl

np.import_array()

def dictToUtf8(inputDict):
    outputdict = {}
    for a,b in inputDict.items():
        key = a.decode('utf-8')
        value = b.decode('utf-8')
        outputdict[key]=value
    return outputdict


cdef class CommonSystem:
    def store(self,name):
        bs = bytes(name,'utf-8')
        self._c_s.store(bs)

    def set_store_interval(self, interval):
        self._c_s.getStoreHandlerP().setStoreInterval(interval)

    def add_break_greater(self,name,value):
        """Add a break that will be activated if the value of the variable
        or state is larger than the value supplied as argument
        """
        bname = bytes(name,'utf-8')
        self._c_s.add_compare_greater(bname,value)

    def add_break_smaller(self,name,value):
        """Add a break that will be activated if the value of the variable
        or state is smaller than the value supplied as argument
        """
        bname = bytes(name,'utf-8')
        self._c_s.add_compare_smaller(bname,value)

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

cdef class Parameters:
    """Contains all the parameters for the system.
    """

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = Parameters()
        p._c_sys = ptr
        return p

    def __dir__(self):
        parstringnames = self._c_sys.getParStringNames()
        parstringnames_uc = [s.decode('utf-8') for s in parstringnames]
        parmapnames = self._c_sys.getParMapNames()
        parmapnames_uc = [s.decode('utf-8') for s in parmapnames]
        parmatrixnames = self._c_sys.getParMatrixNames()
        parmatrixnames_uc = [s.decode('utf-8') for s in parmatrixnames]
        return parstringnames_uc

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        parstringnames =  list(self._c_sys.getParStringNames())
        parmapnames = list(self._c_sys.getParMapNames())
        parmatrixnames = list(self._c_sys.getParMatrixNames())
        if bs in parstringnames:
            return self._c_sys.getParString(bs).decode("utf-8")
        elif bs in parmapnames:
            return self._c_sys.getParMap(bs)
        elif bs in parmatrixnames:
            return self._c_sys.getParMatrix(bs)
        else:
            raise AttributeError("No parameter {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        parstringnames =  list(self._c_sys.getParStringNames())
        parmapnames = list(self._c_sys.getParMapNames())
        parmatrixnames = list(self._c_sys.getParMatrixNames())
        if bs in parstringnames:
            try:
                self._c_sys.setParString(bs,bytes(value,'utf-8'))
            except TypeError:
                raise TypeError("Parameter '{}' is a string".format(name))
        elif bs in parmapnames:
            try:
                self._c_sys.setParMap(bs,value)
            except TypeError:
                raise TypeError("Parameter '{}' is a map".format(name))
        elif bs in parmatrixnames:
            try:
                self._c_sys.setParMatrix(bs,value)
            except TypeError:
                raise TypeError("Parameter '{}' is a matrix".format(name))
        else:
            raise AttributeError("No input {} in system".format(name))

    def get_description(self,varname):
        return self._c_sys.getParDescriptionMap()[varname]

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

    def __repr__(self):
        d = self._getInputDescriptionMap()
        formatstring = "{:>10}  {:>8.3}  {}"
        s = [formatstring.format("name","value","description")]
        s.append("----------  --------  -------------")
        for name,desc in d.items():
            value = self.__getattr__(name)
            printvalues = []
            if type(value)==float:
                printvalues = [value]
            elif type(value[0]) == float:
                printvalues = value
            else:
                printvalues = ["---"]
            shortdesc = desc.partition('\n')[0]
            s.append(formatstring.format(name,printvalues[0],shortdesc))
            for v in printvalues[1:]:
                s.append(formatstring.format("",v,""))
        sout = "\n".join(s)
        return sout

    def find(self,word):
        d = self._sys._getInputDescriptionMap()
        found = False
        for name,desc in d.items():
            if (name.find(word) > -1) or (desc.find(word) > -1):
                found = True
                print("{}\t{}".format(name,desc))

    def get_description(self,varname):
        return self._getInputDescriptionMap[varname]

    def _getInputDescriptionMap(self):
        dutf8 = dictToUtf8(self._c_sys.getInputDescriptionMap())
        return dutf8

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
        scalarnames = self._c_sys.getScalarStateNames()
        scalarnames_uc = [s.decode('utf-8') for s in scalarnames]
        vectornames = self._c_sys.getStateVectorNames()
        vectornames_uc = [s.decode('utf-8') for s in vectornames]
        return scalarnames_uc+vectornames_uc

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getStateVectorNames())
        allscalarnames =  list(self._c_sys.getScalarStateNames())
        if bs in allvectornames:
            return self._c_sys.getStateVector(bs)
        elif bs in allscalarnames:
            return self._c_sys.getScalarState(bs)
        else:
            raise AttributeError("No State {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getStateVectorNames())
        allscalarnames =  list(self._c_sys.getScalarStateNames())
        if bs in allvectornames:
            try:
                self._c_sys.setStateVector(bs,value)
            except TypeError:
                raise TypeError("State '{}' is a vector".format(name))
        elif bs in allscalarnames:
            try:
                self._c_sys.setScalarState(bs,value)
            except TypeError:
                raise TypeError("State '{}' is a scalar".format(name))
        else:
            raise AttributeError("No State {} in system".format(name))

    def __repr__(self):
        d = self._getStateDescriptionMap()
        formatstring = "{:>10}  {:>8.3}  {}"
        s = [formatstring.format("name","value","description")]
        s.append("----------  --------  -------------")
        for name,desc in d.items():
            value = self.__getattr__(name)
            printvalues = []
            if type(value)==float:
                printvalues = [value]
            elif type(value[0]) == float:
                printvalues = value
            else:
                printvalues = ["---"]
            shortdesc = desc.partition('\n')[0]
            s.append(formatstring.format(name,printvalues[0],shortdesc))
            for v in printvalues[1:]:
                s.append(formatstring.format("",v,""))
        sout = "\n".join(s)
        return sout

    def find(self,word):
        d = self._sys._getStateDescriptionMap()
        found = False
        for name,desc in d.items():
            if (name.find(word) > -1) or (desc.find(word) > -1):
                found = True
                print("{}\t{}".format(name,desc))

    def get_description(self,varname):
        return self._getStateDescriptionMap[varname]

    def _getStateDescriptionMap(self):
        dutf8 = dictToUtf8(self._c_sys.getStateDescriptionMap())
        return dutf8

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
        scalarnames = self._c_sys.getScalarDerNames()
        scalarnames_uc = [s.decode('utf-8') for s in scalarnames]
        vectornames = self._c_sys.getDerVectorNames()
        vectornames_uc = [s.decode('utf-8') for s in vectornames]
        return scalarnames_uc+vectornames_uc

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getDerVectorNames())
        allscalarnames =  list(self._c_sys.getScalarDerNames())
        if bs in allvectornames:
            return self._c_sys.getDerVector(bs)
        elif bs in allscalarnames:
            return self._c_sys.getScalarDer(bs)
        else:
            raise AttributeError("No Der {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._c_sys.getDerVectorNames())
        allscalarnames =  list(self._c_sys.getScalarDerNames())
        if bs in allvectornames:
            try:
                self._c_sys.setDerVector(bs,value)
            except TypeError:
                raise TypeError("Der '{}' is a vector".format(name))
        elif bs in allscalarnames:
            try:
                self._c_sys.setScalarDer(bs,value)
            except TypeError:
                raise TypeError("Der '{}' is a scalar".format(name))
        else:
            raise AttributeError("No Der {} in system".format(name))

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
