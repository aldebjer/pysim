"""Contains default systems that can be used in simulations
"""

import pysim.cppsystem

from pysim.systems.defaultsystemcollection1 import _setupSystem
from pysim.systems.defaultsystemcollection1 import _getSystemNames
from pysim.systems.defaultsystemcollection1 import _getSystemDocs

__copyright__ = 'Copyright (c) 2014 SSPA Sweden AB'


def setup_my_system(system):
    """function for preparing and set up a system"""
    _setupSystem(system, system.__class__.__name__)

for name in _getSystemNames():
    docstring = _getSystemDocs(name)
    globals()[name] = type(name,
                           (pysim.cppsystem.Sys,),
                           {'__init__': setup_my_system, "__doc__":docstring})
