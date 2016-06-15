from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

cdef extern from "CommonSystemImpl.hpp":
    cdef cppclass CommonSystemImpl:
        vector[string] getInputVectorNames()
        vector[double] getInputVector(char* name)
        void setInputVector(char*, vector[double])
        map[string,string] getInputDescriptionMap()


cdef extern from "CythonSystemImpl.hpp":
    cdef cppclass CythonSystemImpl(CommonSystemImpl):
        vector[double*] states
        vector[double*] ders
        void* sysp
        void add_input_vector(string, size_t)


cdef class Sys:
    cdef CythonSystemImpl * _c_sys
