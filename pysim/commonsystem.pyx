""" Systems used for simulation.

Base classes for systems. They are inherited by :obj:`CppSystems` and 
:obj:`CythonSystems`.
"""

from collections import namedtuple
from libcpp.vector cimport vector

import numpy as np
cimport numpy as np
from commonsystem cimport CommonSystemImpl
cimport simulatablesystem
cimport cppsystem
cimport cythonsystem


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
    pars : :obj:`Parameters`
        The parameters, can be set before the simulation starts.
    inputs : :obj:`PysimVars`
        The inputs to the system, can be set and connected to outputs
    outputs : :obj:`PysimVars`
        the outputs of the system, can be connected to inputs of other systems
    states : :obj:`PysimVars`
        The states of the system
    ders : :obj:`PysimVars`
        The derivatives of the system
    connections : :obj:`pysim.connections.Connections`
        The connections from this system to other systems
    res : :obj:`Results`
        The stored values from the simulation.        

    """
    def __cinit__(self):
        self._subsystems = {}

    def __getattr__(self, name):
        """
        Return subsystem if one found mathing name otherwise revert to normal
        """
        try:
            bs = bytes(name,'utf-8')
            return self._get_subsystem(self._c_s.get_subsystem(bs))
        except ValueError:
            return super().__getattr__(name)

    def __iter__(self):
        for name in self._c_s.subsystem_names:
            yield self._get_subsystem(self._c_s.get_subsystem(name))

    cdef _get_subsystem(self, simulatablesystem.SimulatableSystemInterface* sub_p):
        cdef cppsystem.CppSystem* c_p
        cdef cythonsystem.CythonSystemImpl* cy_p
        cdef cythonsystem.Sys s

        try:
            c_p = <cppsystem.CppSystem*?> sub_p
            return cppsystem.Sys._create(c_p)
        except TypeError:
            cy_p = <cythonsystem.CythonSystemImpl*?> sub_p
            s = <cythonsystem.Sys> cy_p.sysp
            return s


    def add_subsystem(self, simulatablesystem.SimulatableSystem subsystem, name):
        bs = bytes(name,'utf-8')
        self._c_s.add_subsystem(subsystem._SimulatableSystemInterface_p, bs)
        self._subsystems[name] = subsystem

    def store(self,name):
        """Store a input, output or state in the system.

        Parameters
        ----------
        name : str
            Name of the variable to store.
        """

        bs = bytes(name,'utf-8')
        self._c_s.store(bs)
        self.stores.append(name)
        
    def store_all(self):
        """Store all inputs, outputs, states and ders in the system.
        """
        for input in dir(self.inputs):
            self.store(input)

        for state in dir(self.states):
            self.store(state)

        for der in dir(self.ders):
            self.store(der)

        for output in dir(self.outputs):
            self.store(output)

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
        or state is larger than the value supplied as argument.

        Parameters
        ----------
        name : str
             Name of the variable to watch.
        value: float
             If the variable is below this value then break
  
        """
        bname = bytes(name,'utf-8')
        self._c_s.add_compare_greater(bname,value)

    def add_break_smaller(self,name,value):
        """Add a break that will be activated if the value of the variable
        or state is smaller than the value supplied as argument.
        
        Parameters
        ---------- 
            name : str
                Name of the variable to watch
            value : double
                If the variable is below this value then break

        """
        bname = bytes(name,'utf-8')
        self._c_s.add_compare_smaller(bname,value)


cdef class Results:
    """ Class containing the results of a simulation.

    The results are stored as attributes and returned as numpy arrays. They
    can be scalars, vectors or matrices.
    """

    @staticmethod 
    cdef _create(StoreHandler* ptr):
        p = Results()
        p.shp =ptr
        return p

    def __dir__(self):
        names = self.shp.getStoreNames()
        matnames = self.shp.getStoreMatricesNames()
        unicodenames = [s.decode('utf-8') for s in names]
        unicodematnames = [s.decode('utf-8') for s in matnames]
        return ["time"]+unicodenames + unicodematnames

    def __getattr__(self,name):
        cdef np.ndarray[np.double_t,ndim=1, mode='c'] a
        cdef np.ndarray[np.double_t,ndim=2, mode='c'] a_mat
        cdef np.ndarray[np.double_t,ndim=3, mode='c'] a_vecmat
        names = self.shp.getStoreNames()
        unicodenames = [s.decode('utf-8') for s in names]
        matnames = self.shp.getStoreMatricesNames()
        unicodematnames = [s.decode('utf-8') for s in matnames]
        size = self.shp.getStoreSize()
        bs = bytes(name,'utf-8')
        if name == "time":
            a = np.zeros(size, dtype=np.float64) 
            self.shp.fillWithTime(&a[0])
            return a
        elif name in unicodenames:
            columns = self.shp.getStoreColumns(bs)
            a_mat = np.ones([size,columns],dtype=np.float64)*np.nan
            self.shp.fillWithStore(bs,&a_mat[0,0],size,columns)
            if columns == 1:
                return a_mat[:,0]
            else:
                return a_mat
        elif name in unicodematnames:
            rowcols = self.shp.getStoreRowColumns(bs)
            a_vecmat = np.ones([size, rowcols.first, rowcols.second],dtype=np.float64)*np.nan
            self.shp.fillWithMatrices(bs, &a_vecmat[0,0,0], size, rowcols.first, rowcols.second)
            return a_vecmat
        else:
            raise AttributeError("No stored {} in system".format(name))

ctypedef vector[double] ParVectorType
ctypedef vector[vector[double]] ParMatrixType
ctypedef map[string,double] ParMapType
ctypedef map[string,vector[double]] ParVectorMap

cdef class Parameters:
    """Contains all the parameters for the system.
    """

    @staticmethod 
    cdef _create(CommonSystemImpl* ptr):
        p = Parameters()
        p._c_sys = ptr
        return p

    def __dir__(self):
        allnames = []
        allnames.extend(self._c_sys.getParNames[string]())
        allnames.extend(self._c_sys.getParNames[ParMapType]())
        allnames.extend(self._c_sys.getParNames[ParVectorMap]())
        allnames.extend(self._c_sys.getParNames[ParMatrixType]())
        allnames.extend(self._c_sys.getParNames[ParVectorType]())

        allnames_uc = [s.decode('utf-8') for s in allnames]
        return allnames_uc

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        parstringnames =  list(self._c_sys.getParNames[string]())
        parvectornames =  list(self._c_sys.getParNames[ParVectorType]())
        parmapnames = list(self._c_sys.getParNames[ParMapType]())
        parvectormapnames = list(self._c_sys.getParNames[ParVectorMap]())
        parmatrixnames = list(self._c_sys.getParNames[ParMatrixType]())
        if bs in parstringnames:
            return self._c_sys.getPar[string](bs).decode("utf-8")
        elif bs in parvectornames:
            return self._c_sys.getPar[ParVectorType](bs)
        elif bs in parmapnames:
            bytes_dict = self._c_sys.getPar[ParMapType](bs)
            utf_8_dict = {x.decode('utf-8'):v for x,v in bytes_dict.items()}
            return utf_8_dict
        elif bs in parvectormapnames:
            bytes_dict = self._c_sys.getPar[ParVectorMap](bs)
            utf_8_dict = {x.decode('utf-8'):v for x,v in bytes_dict.items()}
            return utf_8_dict
        elif bs in parmatrixnames:
            return self._c_sys.getPar[ParMatrixType](bs)
        else:
            raise AttributeError("No parameter {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        parstringnames =  list(self._c_sys.getParNames[string]())
        parvectornames =  list(self._c_sys.getParNames[ParVectorType]())
        parmapnames = list(self._c_sys.getParNames[ParMapType]())
        parvectormapnames = list(self._c_sys.getParNames[ParVectorMap]())
        parmatrixnames = list(self._c_sys.getParNames[ParMatrixType]())

        if bs in parstringnames:
            try:
                self._c_sys.setPar[string](bs,bytes(value,'utf-8'))
            except TypeError:
                raise TypeError("Parameter '{}' is a string".format(name))
        elif bs in parmapnames:
            try:
                bytesmap = {bytes(x,'utf-8'):v for x,v in value.items()}
                self._c_sys.setPar[ParMapType](bs,bytesmap)
            except TypeError:
                raise TypeError("Parameter '{}' is a map".format(name))
        elif bs in parvectormapnames:
            try:
                bytesmap = {bytes(x,'utf-8'):v for x,v in value.items()}
                self._c_sys.setPar[ParVectorMap](bs,bytesmap)
            except TypeError:
                raise TypeError("Parameter '{}' is a vector map".format(name))
        elif bs in parmatrixnames:
            try:
                self._c_sys.setPar[ParMatrixType](bs,value)
            except TypeError:
                raise TypeError("Parameter '{}' is a matrix".format(name))
        elif bs in parvectornames:
            try:
                self._c_sys.setPar[ParVectorType](bs,value)
            except TypeError:
                raise TypeError("Parameter '{}' is a vector".format(name))
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
        matrixnames = self._var_p.getMatrixNames()
        matrixnames_uc = [s.decode('utf-8') for s in matrixnames]
        return scalarnames_uc+vectornames_uc+ matrixnames_uc

    def __getattr__(self,name):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._var_p.getVectorNames())
        allscalarnames =  list(self._var_p.getScalarNames())
        allmatrixnames =  list(self._var_p.getMatrixNames())
        if bs in allvectornames:
            return self._var_p.getVector(bs)
        elif bs in allscalarnames:
            return self._var_p.getScalar(bs)
        elif bs in allmatrixnames:
            return self._var_p.getMatrix(bs)
        else:
            raise AttributeError("No variable {} in system".format(name))

    def __setattr__(self,name,value):
        bs = bytes(name,'utf-8')
        allvectornames =  list(self._var_p.getVectorNames())
        allscalarnames =  list(self._var_p.getScalarNames())
        allmatrixnames =  list(self._var_p.getMatrixNames())
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
        elif bs in allmatrixnames:
            try:
                self._var_p.setMatrix(bs,value)
            except TypeError:
                raise TypeError("Variable '{}' is a Matrix".format(name))
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
