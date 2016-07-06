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

cdef _setupSystem(pysim.cppsystem.Sys system,name):
    bs = bytes(name,'utf-8')
    cdef pysim.cppsystem.CppSystem* sp = getCppSystem(bs)
    if sp is NULL:
            raise ValueError("No Such System")
    system._c_sys = sp
    system._c_s = sp
    pysim.cppsystem.Sys._setupParVar(system)

cdef setup_my_system(system):
    """function for preparing and set up a system"""
    _setupSystem(system, system.__class__.__name__)

for name in _getSystemNames():
    docstring = _getSystemDocs(name)
    globals()[name] = type(name,
                           (pysim.cppsystem.Sys,),
                           {'__init__': setup_my_system, "__doc__":docstring})
