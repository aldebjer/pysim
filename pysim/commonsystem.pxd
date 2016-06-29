from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map

cdef extern from "CommonSystemImpl.hpp":
    cdef cppclass CommonSystemImpl:
        vector[string] getInputVectorNames()
        vector[double] getInputVector(char* name)
        void setInputVector(char*, vector[double])
        map[string,string] getInputDescriptionMap()

        vector[string] getOutputVectorNames()
        vector[double] getOutputVector(char* name)
        void setOutputVector(char*, vector[double])
        map[string,string] getOutputDescriptionMap()

        vector[string] getScalarStatetNames()
        vector[string] getStateVectorNames()
        double getState(char* name)
        vector[double] getStateVector(char* name)
        void setStateVector(char*, vector[double])
        map[string,string] getStateDescriptionMap()

        vector[string] getScalarDerNames()
        vector[string] getDerVectorNames()
        double getDer(char* name)
        vector[double] getDerVector(char* name)
        void setDerVector(char*, vector[double])
        map[string,string] getDerDescriptionMap()

        void store(char* name)

        void connect(char*, CommonSystemImpl*, char* );

cdef class CommonSystem:
    cdef CommonSystemImpl * _c_s

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
