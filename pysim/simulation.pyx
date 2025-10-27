"""The simulation module contains a Sim object that represents the simulation.
The systems added to the Sim object will be used when the simulation is used.
The module also contains a number of solvers that the Sim object can use.
"""

from libcpp.vector cimport vector
from libcpp cimport bool
from pysim.commonsystem cimport CommonSystem
from pysim.compositesystem import CompositeSystem
from pysim cimport simulatablesystem
import json
import importlib
import collections
from pysim.simulatablesystem cimport SimulatableSystem


cdef extern from "CppSimulation.hpp" namespace "pysim":
    cdef cppclass Simulation:
        void simulate(double endtime, double dt, char* solvername,double abs_err, double rel_err, bool dense_output) except +
        void addSystem(simulatablesystem.SimulatableSystemInterface* system)
        double getCurrentTime()

class Runge_Kutta_4:
    """A Runge Kutta 4 solver  that can be used when calculating
     a simulation"""

    name = "rk4"
    def __cinit__(self):
        pass

class Cash_Karp:
    """An adaptive steplength solver that can be used when calculating
     a simulation"""

    name = "ck54"

    def __init__(self):
        self.absolute_error = 1e-10
        self.relative_error = 1e-4

class Dormand_Prince_5:
    """An adaptive steplength solver that can be used when calculating
     a simulation"""

    name = "dp5"

    def __init__(self):
        self.absolute_error = 1e-10
        self.relative_error = 1e-4
        self.dense_output = True

def _get_system_name(systemdict, system):
    for name,subsys in systemdict.items():
        if id(subsys) == id(system):
            return name
    raise ValueError('argument is not a system')

cdef class Sim:
    """This class represents an entire simulation. 
       Each simulation consists of one or more Systems that can be added
       to this class
       
       Attributes
       ----------
       systems: OrderedDict
                The systems added to the simulation
                

       
       """
    cdef Simulation * _c_sim
    cdef readonly object systems_list
    cdef readonly object systems

    def __cinit__(self):
        self.systems_list = []
        self.systems = collections.OrderedDict()
        self._c_sim = new Simulation()

    def __dealloc__(self):
        del self._c_sim

    def add_system(self, SimulatableSystem sys not None, name = None):
        """Add a system that will participate in this simulation"""

        #If no name is given give the system instance a name as per:
        #classname, classname_2, classname_3, etc.
        if name is None:
            classname = type(sys).__name__.lower()
            if not (classname in self.systems):
                name = classname
            else:
                for i in range(2,1000):
                    newname = "{}_{}".format(classname,i)
                    if not (newname in self.systems):
                        name = newname
                        break

        self._c_sim.addSystem(sys._SimulatableSystemInterface_p)
        self.systems[name] = sys

    def get_time(self):
        """Get the current time of the simulation.
        Before any simulation has been run this is 0, but after each
        simulation run it is increased
        """
        currentTime = self._c_sim.getCurrentTime()
        return currentTime

    def _save_system(self,system, namedict):
        systemdict = collections.OrderedDict()

        if isinstance(system, CompositeSystem):
            systemdict["type"] = "CompositeSystem"
            systemdict["module"] = "pysim.compositesystem"
            systemdict["ports"] = {"in": system.in_ports, "out": system.out_ports}

            subsystems = {}
            for subname,subsys in system.subsystems.items():
                subsystems[subname] = self._save_system(subsys, system.subsystems)
            systemdict["subsystems"] = subsystems
        else:
            systemdict["type"] = type(system).__name__
            systemdict["module"] = type(system).__module__

        inputdict = collections.OrderedDict()
        for input_name in dir(system.inputs):
            inputdict[input_name] = getattr(system.inputs,input_name)
        systemdict["inputs"]=inputdict

        cons = []
        for c in system.connections.connection_list:
            sysname = _get_system_name(namedict, c[1])
            cons.append((c[0],sysname, c[2]))
        systemdict["connections"] = cons

        systemdict["stores"] = system.stores

        return systemdict

    def save_config(self,filepath):
        """Stores the simulations systems and their input values to a file.
        The path for the file to be stored at is given by the arguement'
        "filepath". This file can later be read with the function "load_config".
        """
        systems_dict = collections.OrderedDict()
        for name,system in self.systems.items():
            systems_dict[name] = self._save_system(system, self.systems)

        root_dict = {"systems":systems_dict}

        with open(filepath,'w') as f:
            json.dump(root_dict,
                      f,
                      sort_keys=True,
                      indent=4,
                      separators=(',', ': '))

    def _setup_system(self, sys_dict):
        typename = sys_dict["type"]
        modulename = sys_dict["module"]
        mod = importlib.import_module(modulename)
        system = getattr(mod,typename)()

        if "ports" in sys_dict.keys():
            for name, value in sys_dict["ports"]["in"].items():
                if value["type"] == "scalar":
                    system.add_port_in_scalar(name, value["value"], value["description"])
            for name, value in sys_dict["ports"]["out"].items():
                if value["type"] == "scalar":
                    system.add_port_out_scalar(name, value["value"], value["description"])

        for iname,ivalue in sys_dict["inputs"].items():
            setattr(system.inputs,iname,ivalue)
        if "subsystems" in sys_dict.keys():
            for subsysname, subsys_dict in sys_dict["subsystems"].items():
               subsys = self._setup_system(subsys_dict)
               system.add_subsystem(subsys, subsysname)
        return system

    def _connect_system(self, system, sys_dict, siblings):
        for con in sys_dict["connections"]:
            system.connections.add_connection(con[0], siblings[con[1]], con[2])

        if "ports" in sys_dict.keys():
            for name, portdict in sys_dict["ports"]["in"].items():
                for con in portdict["connections"]:
                    subsys = system.subsystems[con["subsystem"]]
                    system.connect_port_in(name, subsys, con["input"])
            for name, portdict in sys_dict["ports"]["out"].items():
                for con in portdict["connections"]:
                    subsys = system.subsystems[con["subsystem"]]
                    system.connect_port_out(name, subsys, con["output"])

        if "subsystems" in sys_dict.keys():
            for subsysname, subsys_dict in sys_dict["subsystems"].items():
                self._connect_system(system.subsystems[subsysname], subsys_dict, system.subsystems)

    def load_config(self,filepath):
        """Loads a number of systems and their input values from a file.
        The file is loaded from the path supplied as an argument to the 
        function
        """
        d = collections.OrderedDict()
        with open(filepath,'r') as f:
            d = json.load(f,object_pairs_hook=collections.OrderedDict)
        systems_dict = d["systems"]
        for name,sys_dict in systems_dict.items():
            system = self._setup_system(sys_dict)
            self.add_system(system, name)

        for name,sys_dict in systems_dict.items():
            self._connect_system(self.systems[name], sys_dict, self.systems)


    def simulate(self, double duration, double dt, solver = Runge_Kutta_4() ):
        """Start or continue a simulation for duration seconds.
        The default ODE-solver used is the classic fixed steplength
        Runge Kutta 4 (rk4), but it is also possible to use other solvers by
        supplying the solver in the argument "solver".
        For fixed lenght algorithms the default time step is 0.1 seconds.
        """
        name  = bytes(solver.name,'utf-8')
        abs_err = 0
        rel_err = 0
        dense = False
        if isinstance(solver, Cash_Karp) or isinstance(solver,Dormand_Prince_5):
            abs_err = solver.absolute_error
            rel_err = solver.relative_error
        if isinstance(solver,Dormand_Prince_5):
            dense = solver.dense_output

        self._c_sim.simulate(duration, dt, name,abs_err,rel_err,dense)
