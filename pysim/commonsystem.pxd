from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map

cdef extern from "CommonSystemImpl.hpp" namespace "pysim":
    cdef cppclass CommonSystemImpl:

        vector[string] getParStringNames()
        vector[string] getParMatrixNames()
        vector[string] getParMapNames()
        void setParString(char*, string) except +
        string getParString(char*) except +
        vector[vector[double]] getParMatrix(char* name) except +
        void setParMatrix(char* name, vector[vector[double]] value) except +
        map[string,double] getParMap(char* name) except +
        void setParMap(char* name, map[string,double] value) except +
        map[string,string] getParDescriptionMap() except +

        vector[string] getInputVectorNames()
        vector[double] getInputVector(char* name) except +
        void setInputVector(char*, vector[double]) except +
        vector[string] getScalarInputNames()
        void setScalarInput(char*, double) except +
        double getScalarInput(char*) except +
        map[string,string] getInputDescriptionMap()

        vector[string] getOutputVectorNames()
        vector[double] getOutputVector(char* name) except +
        void setOutputVector(char*, vector[double]) except +
        vector[string] getScalarOutputNames()
        void setScalarOutput(char*, double) except +
        double getScalarOutput(char*) except +
        map[string,string] getOutputDescriptionMap()

        vector[string] getStateVectorNames()
        vector[double] getStateVector(char* name) except +
        void setStateVector(char*, vector[double]) except +
        vector[string] getScalarStateNames()
        void setScalarState(char*, double) except +
        double getScalarState(char* name)
        map[string,string] getStateDescriptionMap()

        vector[string] getDerVectorNames()
        vector[double] getDerVector(char* name) except +
        void setDerVector(char*, vector[double]) except +
        vector[string] getScalarDerNames()
        void setScalarDer(char* name, double) except +
        double getScalarDer(char* name)
        map[string,string] getDerDescriptionMap()

        void store(char* name)
        StoreHandler* getStoreHandlerP()

        void add_compare_greater(char* comparename, double comparevalue) except +
        void add_compare_smaller(char* comparename, double comparevalue) except +

        void connect(char*, CommonSystemImpl*, char* );

cdef extern from "StoreHandler.hpp" namespace "pysim":
    cdef cppclass StoreHandler:
        int getStoreSize()  except +
        int getStoreColumns(char* name) except +
        void fillWithStore(char* name, double* p, int rows, int columns) except +
        void fillWithTime(double* p) except +
        vector[string] getStoreNames()
        void setStoreInterval(double interval)


cdef class CommonSystem:
    cdef CommonSystemImpl * _c_s

cdef class Parameters:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class Inputs:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class Outputs:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class States:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class Ders:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class Connections:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class Results:
    cdef CommonSystemImpl* _c_sys
    cdef StoreHandler* shp
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)
