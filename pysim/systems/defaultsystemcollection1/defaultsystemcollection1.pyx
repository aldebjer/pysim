from libcpp.string cimport string
from libcpp.vector cimport vector

cimport pysim.cppsystem
import pysim.cppsystem

cdef extern from "cppsource/factory.hpp":
    pysim.cppsystem.CppSystem* getCppSystem(char* name)
    vector[string] getCppSystemNames()
    string getCppSystemDocs(char* name)

def _getSystemNames():
    cdef vector[string] v = getCppSystemNames()
    return [l.decode("utf-8") for l in v]

def _getSystemDocs(name):
    bs = bytes(name,'utf-8')
    docstringbytes = getCppSystemDocs(bs)
    docstring = docstringbytes.decode("utf-8")
    return docstring

def _setupSystem(pysim.cppsystem.Sys sys,name):
    bs = bytes(name,'utf-8')
    sys._c_sys = getCppSystem(bs)
    if sys._c_sys is NULL:
            raise ValueError("No Such System")
    pysim.cppsystem.Sys._setupParVar(sys)
