from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp.pair cimport pair

cimport simulatablesystem
from connections cimport ConnectionHandler

cdef extern from "Variable.hpp" namespace "pysim":
    cdef cppclass Variable:
        vector[string] getMatrixNames()
        vector[vector[double]] getMatrix(char* name) except +
        void setMatrix(char*, vector[vector[double]]) except +
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

        vector[string] getParNames[T]()
        T getPar[T](char*) except +
        void setPar[T](char* name, T value) except +
        map[string,string] getParDescriptionMap() except +

        void store(const char* name) except +
        StoreHandler* getStoreHandlerP()

        void add_compare_greater(char* comparename, double comparevalue) except +
        void add_compare_smaller(char* comparename, double comparevalue) except +

cdef extern from "StoreHandler.hpp" namespace "pysim":
    cdef cppclass StoreHandler:
        size_t getStoreSize()  except +
        size_t getStoreColumns(char* name) except +
        pair[size_t, size_t] getStoreRowColumns(char* name) except +
        void fillWithStore(char* name, double* p, size_t rows, size_t columns) except +
        void fillWithTime(double* p) except +
        void fillWithMatrices(char* name, double* p, size_t timesteps, size_t rows, size_t columns) except +
        vector[string] getStoreNames()
        vector[string] getStoreMatricesNames()
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
