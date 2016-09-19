from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map

cimport simulatablesystem
from connections cimport ConnectionHandler

cdef extern from "Variable.hpp" namespace "pysim":
    cdef cppclass Variable:
        vector[string] getVectorNames()
        vector[double] getVector(char* name) except +
        void setVector(char*, vector[double]) except +
        vector[string] getScalarNames()
        void setScalar(char*, double) except +
        double getScalar(char*) except +
        map[string,string] getDescriptionMap()

cdef extern from "CommonSystemImpl.hpp" namespace "pysim":
    cdef cppclass CommonSystemImpl(simulatablesystem.SimulatableSystemInterface):

        Variable inputs
        Variable outputs
        Variable states
        Variable ders
        ConnectionHandler connectionHandler

        vector[string] getParStringNames()
        vector[string] getParVectorNames()
        vector[string] getParMatrixNames()
        vector[string] getParMapNames()
        void setParString(char*, string) except +
        string getParString(char*) except +
        vector[vector[double]] getParMatrix(char* name) except +
        void setParMatrix(char* name, vector[vector[double]] value) except +
        vector[double] getParVector(char* name) except +
        void setParVector(char* name, vector[double] value) except +
        map[string,double] getParMap(char* name) except +
        void setParMap(char* name, map[string,double] value) except +
        map[string,string] getParDescriptionMap() except +

        void store(char* name)
        StoreHandler* getStoreHandlerP()

        void add_compare_greater(char* comparename, double comparevalue) except +
        void add_compare_smaller(char* comparename, double comparevalue) except +

cdef extern from "StoreHandler.hpp" namespace "pysim":
    cdef cppclass StoreHandler:
        int getStoreSize()  except +
        int getStoreColumns(char* name) except +
        void fillWithStore(char* name, double* p, int rows, int columns) except +
        void fillWithTime(double* p) except +
        vector[string] getStoreNames()
        void setStoreInterval(double interval)




cdef class CommonSystem(simulatablesystem.SimulatableSystem):
    cdef CommonSystemImpl * _c_s

cdef class Parameters:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class PysimVars:
    cdef Variable* _var_p
    @staticmethod
    cdef _create(Variable* var_ptr)

cdef class Results:
    cdef StoreHandler* shp
    @staticmethod
    cdef _create(StoreHandler* ptr)
