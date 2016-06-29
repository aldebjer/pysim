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

cdef class Inputs:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)

cdef class Outputs:
    cdef CommonSystemImpl* _c_sys
    @staticmethod
    cdef _create(CommonSystemImpl* ptr)
