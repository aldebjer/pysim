"""Example systems created in Python
"""

from pysim.cythonsystem import Sys

class VanDerPol(Sys):
    """Simple example of a class representing a VanDerPol oscillator.
    """
    def __init__(self):
        self.add_state("x", "dx")
        self.add_state("y", "dy")
        self.add_input("a")
        self.add_input("b")
        self.inputs.a = 1.0
        self.inputs.b = 1.0
        self.states.x = 1.0
        self.states.y = 0.0

    def do_step(self,dummy):
        """Perform a timestep by implmenting the VanDerPol equations"""
        
        a = self.inputs.a
        b = self.inputs.b
        x = self.states.x
        y = self.states.y

        self.ders.dx = a*x*(b-y*y)-y
        self.ders.dy = x


class MassSpringDamper(Sys):
    """Simple class for testing the mass-spring-damper simulations with 
    a cython system"""

    def __init__(self):
        """Setup two states (one dimensional vectors for now). Initial 
        conditions are simular to those in the build in c++ system"""
        self.add_state("x1", "dx1")
        self.add_state("x2", "dx2")
        self.states.x1 = 1
        self.states.x2 = 0

    def do_step(self,dummy):
        """Perform a step using default constants, same as those in the 
        cpp system"""

        m = 100.0
        b = 1.0
        k = 50.0
        f = 0.0
        x1 = self.states.x1
        x2 = self.states.x2
        self.ders.dx1 = x2
        self.ders.dx2 =-k/m*x1-b/m*x2+1/m*f
