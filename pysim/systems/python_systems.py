"""Example systems created in Python
"""
import numpy as np

from pysim.cythonsystem import Sys

class VanDerPol(Sys):
    """Simple example of a class representing a VanDerPol oscillator.
    """
    def __init__(self):
        self.add_state_scalar("x", "dx")
        self.add_state_scalar("y", "dy")
        self.add_input_scalar("a")
        self.add_input_scalar("b")
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
        self.add_state_scalar("x1", "dx1")
        self.add_state_scalar("x2", "dx2")
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

class InOutTestSystem(Sys):
    """Python representation of the cpp InOutTestSystem

    Used for testing that the cpp system behaves as the python system
    with regards to the input output handling
    """
    def __init__(self):
        self.add_input_scalar("input_scalar")
        self.add_input_vector("input_vector",3)
        self.add_input_matrix("input_matrix",3,3)

        self.add_state_scalar("state_scalar","der_scalar")
        self.add_state_vector("state_vector","der_vector", 3)
        self.add_state_matrix("state_matrix","der_matrix", 3, 3)

        self.add_output_scalar("input_output_scalar")
        self.add_output_vector("input_output_vector",3)
        self.add_output_matrix("input_output_matrix",3,3)
        self.add_output_scalar("state_output_scalar")
        self.add_output_vector("state_output_vector",3)
        self.add_output_matrix("state_output_matrix",3,3)

        self.inputs.input_scalar = 0.0
        self.inputs.input_vector = [0.0, 0.0, 0.0]
        self.inputs.input_matrix = np.zeros((3,3))

        self.outputs.input_output_scalar = 0.0
        self.outputs.input_output_vector = [0.0, 0.0, 0.0]
        self.outputs.input_output_matrix = np.zeros((3,3))
        self.outputs.state_output_scalar = 0.0
        self.outputs.state_output_vector = [0.0, 0.0, 0.0]
        self.outputs.state_output_matrix = np.zeros((3,3))

        self.states.state_scalar = 1.23
        self.states.state_vector = np.ones(3)*4.56
        self.states.state_matrix = np.ones((3,3))*7.89
        self.ders.der_scalar = 0
        self.ders.der_vector = np.zeros(3)
        self.ders.der_matrix = np.zeros((3,3))

    def do_step(self,dummy):
        """During a timestep we set the outputs to their respective inputs"""
        self.outputs.input_output_scalar = self.inputs.input_scalar
        self.outputs.input_output_vector = self.inputs.input_vector
        self.outputs.input_output_matrix = self.inputs.input_matrix
        self.outputs.state_output_scalar = self.states.state_scalar
        self.outputs.state_output_vector = self.states.state_vector
        self.outputs.state_output_matrix = self.states.state_matrix
