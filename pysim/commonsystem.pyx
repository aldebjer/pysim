""" Systems used for simulation.

Base classes for systems. They are inherited by CppSystems and CythonSystems.
"""

from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from commonsystem cimport CommonSystemImpl
cimport simulatablesystem


np.import_array()

cdef dictToUtf8(inputDict):
    """Convert a dict of bytes to strings.
    It assumes the bytes are in utf-8 format.
    """
    outputdict = {}
    for a,b in inputDict.items():
        key = a.decode('utf-8')
        value = b.decode('utf-8')
        outputdict[key]=value
    return outputdict


cdef class CommonSystem:
    """Common base class for systems that are editable from Python.

    Attributes
    ----------
    pars : Parameters
        The parameters, can be set before the simulation starts.
    inputs : Inputs
        The inputs to the system, can be set and connected to outputs
    outputs : Ouputs
        the outputs of the system, can be connected to inputs of other systems
    states : States
        The states of the system

    """
    def store(self,name):
        """Store a input, output or state in the system.

        Parameters
        ----------
        name : str
            The first parameter.
        """

        bs = bytes(name,'utf-8')
        self._c_s.store(bs)

    def set_store_interval(self, interval):
        """Set the store interval of this system. 

        By default the
        store interval for all systems in a simulation is set by
        the simulation, typically for the step length for fixed
        step algorithms. By calling this function the store interval
        *for this system* is set differently.

        Parameters
        ----------
        interval : float
            The time between stores, in seconds.
        """
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
    cdef _create(StoreHandler* ptr):
        p = Results()
        p.shp =ptr
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

cdef class PysimVars:
    """Contains all the variables for the system.
    
    Each attribute of this class correspond to a variable of the system.
    """

    @staticmethod 
    cdef _create(Variable* ptr):
        p = PysimVars()
        p._var_p = ptr
        return p

    def __dir__(self):
        scalarnames = self._var_p.getScalarNames()
        scalarnames_uc = [s.decode('utf-8') for s in scalarnames]
        vectornames = self._var_p.getVectorNames()
        vectornames_uc = [s.decode('utf-8') for s in vectornames]
        return scalarnames_uc+vectornames_uc

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._var_p.getVectorNames())
        allscalarnames =  list(self._var_p.getScalarNames())
        if bs in allvectornames:
            return self._var_p.getVector(bs)
        elif bs in allscalarnames:
            return self._var_p.getScalar(bs)
        else:
            raise AttributeError("No variable {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._var_p.getVectorNames())
        allscalarnames =  list(self._var_p.getScalarNames())
        if bs in allvectornames:
            try:
                self._var_p.setVector(bs,value)
            except TypeError:
                raise TypeError("Variable '{}' is a vector".format(name))
        elif bs in allscalarnames:
            try:
                self._var_p.setScalar(bs,value)
            except TypeError:
                raise TypeError("Variable '{}' is a scalar".format(name))
        else:
            raise AttributeError("No variable {} in system".format(name))

    def __repr__(self):
        d = self._getDescriptionMap()
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

    def _getDescriptionMap(self):
        dutf8 = dictToUtf8(self._var_p.getDescriptionMap())
        return dutf8
