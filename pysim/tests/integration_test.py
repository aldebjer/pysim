"""This module contains tests for the integration function of PySim
"""

from unittest import TestCase
import numpy as np
from numpy import cos, sin, sqrt

from pysim.simulation import Sim
from pysim.simulation import Runge_Kutta_4
from pysim.simulation import Cash_Karp
from pysim.simulation import Dormand_Prince_5

from pysim.systems import MassSpringDamper

from pysim.systems.python_systems import MassSpringDamper as PyMassSpringDamper

__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

def solve_msd_analytically(t):
    """Returns the analytical solution to the differential equation
    at the timepoints t.
    """
    mass = 100
    springk = 50
    damping = 1
    x0 = 1

    #Calculate help variables
    zeta = damping/(2*sqrt(mass*springk))
    omega0 = sqrt(springk/mass)

    #Solve the differential equation assuming zeta<0, an
    #under dampened system.
    assert zeta < 1
    omegad = omega0*sqrt(1-zeta**2)
    A = x0
    B = (zeta*omega0*x0)/omegad

    y = np.power(np.e, -1*zeta*omega0*t)*(A*cos(omegad*t)+B*sin(omegad*t))
    return y

class IntegrationTest(TestCase):
    """Tests the integration function of a under dampened mass-spring-damper
    system. The differential equation is solved analytically for the system
    and this solution is then compared to the PySim solution.
    """

    def setUp(self):
        self.sim = Sim()
        self.sys = MassSpringDamper()
        self.sys.store("x1")
        self.sim.add_system(self.sys)
        self.integrationlength = 50
        self.timestep = 0.1

    def compare_to_analytical(self, tolerance):
        """Compare the result in the sys to the analytical solution of the
        diff-equation. If the difference is larger than tolerance an assert
        will fail.
        """
        time = self.sys.res.time
        integrated_result = self.sys.res.x1
        analytic_result = solve_msd_analytically(time)

        maxerror = np.max(np.abs(integrated_result-analytic_result))
        print(maxerror)
        assert maxerror < tolerance



    def test_default(self):
        """Test the default sover is a Runge Kutta and compares the results
        solver results for accuracy
        """
        self.sim.simulate(self.integrationlength, self.timestep)
        self.compare_to_analytical(1e-5)

    def test_rk4(self):
        """Test the Runge Kutta solver results for accuracy"""
        solver = Runge_Kutta_4
        self.sim.simulate(self.integrationlength, self.timestep, solver)
        self.compare_to_analytical(1e-5)

    def test_dp5(self):
        """Test the Dormand Price solver results for accuracy when the
        default dense output is used, as well as the default relative
        error of 10^-4
        """
        solver = Dormand_Prince_5()
        self.sim.simulate(self.integrationlength, self.timestep, solver)
        self.compare_to_analytical(1e-3)

    def test_dp5_high_accuracy(self):
        """Test the Dormand Price solver results for accuracy with an increased
        relative error control of 10^-6. The default dense output is used.
        """
        solver = Dormand_Prince_5()
        solver.relative_error = 1e-6
        self.sim.simulate(self.integrationlength, self.timestep, solver)
        self.compare_to_analytical(1e-5)

    def test_dp5_dense_timediff(self):
        """Test that the timesteps delivered from the Dormand Price solver is
        regurarly spaced.
        """
        solver = Dormand_Prince_5()
        self.sim.simulate(self.integrationlength, self.timestep, solver)
        diff = np.diff(self.sys.res.time)
        absdifferr = np.abs(diff-self.timestep)
        assert np.max(absdifferr) < 1e-14


    def test_ck(self):
        """Test the Cash Karp solver results for accuracy with its default
        relative
        error of 10^-4
        """
        solver = Cash_Karp()
        self.sim.simulate(self.integrationlength, self.timestep, solver)
        self.compare_to_analytical(1e-2)

    def test_ck_high_accuracy(self):
        """Test the Cash Karp solver results for accuracy with an increased
        relative error control of 10^-6. The default dense output is used.
        """
        solver = Cash_Karp()
        solver.relative_error = 1e-8
        self.sim.simulate(self.integrationlength, self.timestep, solver)
        self.compare_to_analytical(1e-7)




class CythonIntegrationTest(IntegrationTest):
    """Use a Cython System to simulate. The results are compared with the
    analytical results.
    """

    def setUp(self):
        self.sim = Sim()
        self.sys = PyMassSpringDamper()
        self.sys.store("x1")
        self.sim.add_system(self.sys)
        self.integrationlength = 50
        self.timestep = 0.1
