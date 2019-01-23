"""The simulation module contains a Sim object that represents the simulation.
The systems added to the Sim object will be used when the simulation is used.
The module also contains a number of solvers that the Sim object can use.
"""

from libcpp.vector cimport vector
from libcpp cimport bool
from commonsystem cimport CommonSystem
cimport simulatablesystem
import json
import importlib
import collections
from simulatablesystem cimport SimulatableSystem


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
