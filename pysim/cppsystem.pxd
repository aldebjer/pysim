from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool

cdef extern from "CppSystem.hpp":

    cdef cppclass CppSystem:
        void store(char* name) except +
        int getStoreSize()  except +
        int getStoreColumns(char* name) except +
        void fillWithStore(char* name, double* p, int rows, int columns) except +
        void fillWithTime(double* p) except +
        vector[string] getStoreNames()
        void setStoreInterval(double interval)


        vector[string] getInputNames()
        vector[string] getInputVectorNames()
        vector[string] getInputMatrixNames()
        vector[string] getInputStringNames()
        vector[string] getInputMapNames()

        double getInput(char* name) except +
        vector[double] getInputVector(char* name) except +
        vector[vector[double]] getInputMatrix(char* name) except +
        string getInputString(char* name) except +
        map[string,double] getInputMap(char* name) except +

        void setInput(char* name, double value) except +
        void setInputVector(char* name, vector[double] value) except +
        void setInputMatrix(char* name, vector[vector[double]] value) except +
        void setInputString(char* name, string value) except +
        void setInputMap(char* name, map[string,double] value) except +

        map[string,string] getInputDescriptionMap() except +


        vector[string] getOutputNames()
        vector[string] getOutputVectorNames()

        double getOutput(char* name) except +
        vector[double] getOutputVector(char* name) except +

        map[string,string] getOutputDescriptionMap() except +


        vector[string] getStateNames()
        vector[string] getStateVectorNames()

        double getState(char* name) except +
        vector[double] getStateVector(char* name) except +

        void setState(char* statename, double value)
        void setStateVector(char* statename, vector[double] value) except +

        map[string,string] getStateDescriptionMap() except +

        void connect(char* outputname, CppSystem* inputsys, char* inputname) except +

        void add_compare_greater(char* comparename, double comparevalue) except +
        void add_compare_smaller(char* comparename, double comparevalue) except +
        bool do_comparison() except +



cdef class Sys:
    cdef CppSystem * _c_sys
    cdef public object pars
    cdef public object res

    
