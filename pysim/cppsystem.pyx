from collections import namedtuple

import numpy as np
cimport numpy as np
cimport cppsystem

np.import_array()

def dictToUtf8(inputDict):
    outputdict = {}
    for a,b in inputDict.items():
        key = a.decode('utf-8')
        value = b.decode('utf-8')
        outputdict[key]=value
    return outputdict

class Inputs:
    """Contains all the inputs for the system.
    To list the inputs use input.list()
    To search for a input that contains a specific word, or has a 
    description that contains the word use input.search(word).
    """
    def __init__(self,sys):
        self.__dict__["_sys"] = sys
    def __dir__(self):
        scalarnames = self._sys._getInputNames()
        vectornames = self._sys._getInputVectorNames()
        vectornames = self._sys._getInputMatrixNames()
        stringnames = self._sys._getInputStringNames()
        mapnames = self._sys._getInputMapNames()
        return scalarnames+vectornames+stringnames+mapnames
    def __getattr__(self,name):
        if name in self._sys._getInputNames():
            return self._sys._getInput(name)
        elif name in self._sys._getInputVectorNames():
            return self._sys._getInputVector(name)
        elif name in self._sys._getInputMatrixNames():
            return self._sys._getInputMatrix(name)
        elif name in self._sys._getInputStringNames():
            return self._sys._getInputString(name)
        elif name in self._sys._getInputMapNames():
            return self._sys._getInputMap(name)
        else:
            raise AttributeError("No input {} in system".format(name))
    def __setattr__(self,name,value):
        if name in self._sys._getInputNames():
            try:
                self._sys._setInput(name,value)
            except TypeError:
                raise TypeError("Input '{}' is a scalar".format(name))
        elif name in self._sys._getInputVectorNames():
            try:
                self._sys._setInputVector(name,value)
            except TypeError:
                raise TypeError("Input '{}' is a vector".format(name))
        elif name in self._sys._getInputMatrixNames():
            try:
                self._sys._setInputMatrix(name,value)
            except TypeError:
                raise TypeError("Input '{}' is a matrix".format(name))
        elif name in self._sys._getInputStringNames():
            try:
                self._sys._setInputString(name,value)
            except TypeError:
                raise TypeError("Input '{}' is a string".format(name))
        elif name in self._sys._getInputMapNames():
            try:
                self._sys._setInputMap(name,value)
            except TypeError:
                raise TypeError("Input '{}' is a map".format(name))
        else:
            raise AttributeError("No input {} in system".format(name))

    def __repr__(self):
        d = d = self._sys._getInputDescriptionMap()
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
        return self._sys._getInputDescriptionMap()[varname]


class States:
    def __init__(self,sys):
        self.__dict__["_sys"] = sys
        self.__dict__["_names"] = sys._getStateNames()
        self.__dict__["_vectornames"] = sys._getStateVectorNames()
    def __dir__(self):
        return self._names+self._vectornames
    def __getattr__(self,name):
        if name in self._names:
            return self._sys._getState(name)
        elif name in self._vectornames:
            return self._sys._getStateVector(name)
        else:
            raise AttributeError("No state {} in system".format(name))
    def __setattr__(self,name,value):
        if name in self._names:
            try:
                self._sys._setState(name,value)
            except TypeError:
                raise TypeError("State '{}' is a scalar".format(name))
        elif name in self._vectornames:
            try:
                self._sys._setStateVector(name,value)
            except TypeError:
                raise TypeError("State '{}' is a vector".format(name))
        else:
            raise AttributeError("No state {} in system".format(name))

    def __repr__(self):
        d = d = self._sys._getStateDescriptionMap()
        formatstring = "{:>10}  {:>8.3}  {}"
        s = [formatstring.format("name","value","description")]
        s.append("----------  --------  -------------")
        for name,desc in d.items():
            value = self.__getattr__(name)
            printvalues = []
            if type(value)==float:
                printvalues = [value]
            else:
                printvalues = value
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


class Outputs:
    def __init__(self,sys):
        self.__dict__["_sys"] = sys
        self.__dict__["_names"] = sys._getOutputNames()
        self.__dict__["_vectornames"] = sys._getOutputVectorNames()
    def __dir__(self):
        return self._names+self._vectornames
    def __getattr__(self,name):
        if name in self._names:
            return self._sys._getOutput(name)
        elif name in self._vectornames:
            return self._sys._getOutputVector(name)
        else:
            raise AttributeError("No output {} in system".format(name))

    def __repr__(self):
        d = d = self._sys._getOutputDescriptionMap()
        formatstring = "{:>10}  {:>8.3}  {}"
        s = [formatstring.format("name","value","description")]
        s.append("----------  --------  -------------")
        for name,desc in d.items():
            value = self.__getattr__(name)
            printvalues = []
            if type(value)==float:
                printvalues = [value]
            else:
                printvalues = value
            shortdesc = desc.partition('\n')[0]
            s.append(formatstring.format(name,printvalues[0],shortdesc))
            for v in printvalues[1:]:
                s.append(formatstring.format("",v,""))
        sout = "\n".join(s)
        return sout

    def find(self,word):
        d = self._sys._getOutputDescriptionMap()
        found = False
        for name,desc in d.items():
            if (name.find(word) > -1) or (desc.find(word) > -1):
                found = True
                print("{}\t{}".format(name,desc))

    def get_description(self,varname):
        return self._sys._getOutputDescriptionMap()[varname]

class Results:
    def __init__(self,sys):
        self.__dict__["_sys"] = sys
    def __dir__(self):
        return ["time"]+self._sys._getStoreNames()
    def __getattr__(self,name):
        if name == "time":
            return self._sys._getTime()
        elif name in self._sys._getStoreNames():
            return self._sys._getStore(name)
        else:
            raise AttributeError("No stored {} in system".format(name))
            
cdef class Sys:
    """Represents a System, to be added to an Simulation for evaluation.
       Each System consists of parameters, inputs set prior to the simulation,
       States and State derivatives, and Variables, that may be stored during
       a simulation"""
       

        
    def __dealloc__(self):
        del self._c_sys

    def _setupParVar(self):
        self.inputs = Inputs(self)
        self.inputs_default = self._get_default_inputs()
        self.states = States(self)
        self.res = Results(self)
        self.outputs = Outputs(self)

    def _get_default_inputs(self):
        scalarnames = self._getInputNames()
        scalarvalues = [self._getInput(name) for name in scalarnames]
        vectornames = self._getInputVectorNames()
        vectorvalues = [self._getInputVector(name) for name in vectornames]

        allnames = scalarnames + vectornames
        allvalues = scalarvalues+vectorvalues
        defaults = dict(zip(allnames, allvalues))
        return defaults

    def _setInput(self,name,double value):
        bs = bytes(name,'utf-8')
        self._c_sys.setInput(bs, value)

    def _setInputVector(self,name,vector[double] value):
        bs = bytes(name,'utf-8')
        self._c_sys.setInputVector(bs, value)

    def _setInputMatrix(self,name,vector[vector[double]] value):
        bs = bytes(name,'utf-8')
        self._c_sys.setInputMatrix(bs, value)

    def _setInputString(self,name,value):
        bs = bytes(name,'utf-8')
        vs = bytes(value,'utf-8')
        self._c_sys.setInputString(bs, vs)

    def _setInputMap(self,name,map_in):
        bs = bytes(name,'utf-8')
        bytemap = {bytes(key,'utf-8'):value for (key,value) in map_in.items()}
        self._c_sys.setInputMap(bs, bytemap)

    def _getInput(self,name):
        bs = bytes(name,'utf-8') 
        return self._c_sys.getInput(bs)

    def _getInputVector(self,name):
        bs = bytes(name,'utf-8')
        v = self._c_sys.getInputVector(bs)
        return np.array(v)

    def _getInputMatrix(self,name):
        bs = bytes(name,'utf-8')
        v = self._c_sys.getInputMatrix(bs)
        return np.array(v)

    def _getInputString(self,name):
        bs = bytes(name,'utf-8')
        vs = self._c_sys.getInputString(bs)
        return vs.decode("utf-8")

    def _getInputMap(self,name):
        bs = bytes(name,'utf-8')
        out_map = self._c_sys.getInputMap(bs)
        stringed_map = {key.decode('utf-8'):value for (key,value) in out_map.items()}
        return stringed_map

    def _getInputNames(self):
        names = self._c_sys.getInputNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _getInputVectorNames(self):
        names = self._c_sys.getInputVectorNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _getInputMatrixNames(self):
        names = self._c_sys.getInputMatrixNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _getInputStringNames(self):
        names = self._c_sys.getInputStringNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _getInputMapNames(self):
        names = self._c_sys.getInputMapNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _getInputDescriptionMap(self):
        dutf8 = dictToUtf8(self._c_sys.getInputDescriptionMap())
        return dutf8

    def _getOutput(self,name):
        bs = bytes(name,'utf-8')
        v = self._c_sys.getOutput(bs)
        return v

    def _getOutputVector(self,name):
        bs = bytes(name,'utf-8')
        v = self._c_sys.getOutputVector(bs)
        return np.array(v)

    def _getOutputNames(self):
        names = self._c_sys.getOutputNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _getOutputVectorNames(self):
        names = self._c_sys.getOutputVectorNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _getOutputDescriptionMap(self):
        dutf8 = dictToUtf8(self._c_sys.getOutputDescriptionMap())
        return dutf8




    def _getStateNames(self):
        names = self._c_sys.getStateNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _getStateVectorNames(self):
        names = self._c_sys.getStateVectorNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def _setState(self,name,double value):
        bs = bytes(name,'utf-8')
        self._c_sys.setState(bs, value)

    def _setStateVector(self,name,vector[double] value):
        bs = bytes(name,'utf-8')
        self._c_sys.setStateVector(bs, value)

    def _getState(self,name):
        bs = bytes(name,'utf-8') 
        return self._c_sys.getState(bs)

    def _getStateVector(self,name):
        bs = bytes(name,'utf-8')
        v = self._c_sys.getStateVector(bs)
        return np.array(v)

    def _getStateDescriptionMap(self):
        dutf8 = dictToUtf8(self._c_sys.getStateDescriptionMap())
        return dutf8

    def _getStoreNames(self):
        names = self._c_sys.getStoreNames()
        unicodenames = [s.decode('utf-8') for s in names]
        return unicodenames

    def store(self,name):
        if name in self._getStoreNames():
            raise ValueError("Variable {} allready stored".format(name))
        bs = bytes(name,'utf-8')
        self._c_sys.store(bs)

    def set_store_interval(self, interval):
        self._c_sys.setStoreInterval(interval)

    def _getStore(self,name):
        bs = bytes(name,'utf-8')
        size = self._c_sys.getStoreSize()
        columns = self._c_sys.getStoreColumns(bs)

        cdef np.ndarray[np.double_t,ndim=2, mode='c'] a
        a = np.zeros([size,columns],dtype=np.float64) 
        self._c_sys.fillWithStore(bs,&a[0,0],size,columns)
        if columns == 1:
            return a[:,0]
        else:
            return a

    def _getTime(self):
        size = self._c_sys.getStoreSize()
        cdef np.ndarray[np.double_t,ndim=1, mode='c'] a
        a = np.zeros(size,dtype=np.float64) 
        self._c_sys.fillWithTime(&a[0])
        return a

    def connect(self,outputname,Sys inputsys,inputname):
         bsout =  bytes(outputname,'utf-8')
         bsin =  bytes(inputname,'utf-8')
         self._c_sys.connect(bsout,inputsys._c_sys,bsin)

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
