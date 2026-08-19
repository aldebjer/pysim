__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

from setuptools import Command, Extension, setup
from setuptools.command.sdist import sdist
from Cython.Build import cythonize
import sys
import sysconfig
import numpy
import os

from pysim import __version__

class BuildExeCommand(Command):
    """A custom command to build the pysim c++ executable"""
    description = 'Create the pure c++ pysim executable'
    user_options = []


    def initialize_options(self):
        """Set default values for options."""
        pass

    def finalize_options(self):
        """Post-process options."""
        pass

    def distutils_dir_name(self):
        """Returns the name of a distutils build directory"""
        f = "build/temp.{platform}-{version[0]}.{version[1]}"
        return f.format(platform=sysconfig.get_platform(),
                        version=sys.version_info)

    def run(self):
        from distutils import ccompiler
        compiler = ccompiler.new_compiler()
        compiler.set_include_dirs([os.environ['BOOST_ROOT'],
                                   os.environ['EIGEN_ROOT'],
                                   "pysim/cppsource",
                                  ]
                                 )
        cpplibdir = self.distutils_dir_name()
        compiler.set_library_dirs([cpplibdir])
        compiler.set_libraries(["cppsystemlib"])
        compiler.compile(["msvc/cpp_runner/main.cpp"])
        compiler.link_executable(["msvc/cpp_runner/main.obj"],"cpp_runner")
    


include_dirs = ['pysim/cppsource', numpy.get_include()]

extracompileargs = []
if sys.platform == "win32":
    include_dirs.extend([os.environ['BOOST_ROOT'], os.environ['EIGEN_ROOT']])

if sys.platform in ("linux","darwin"):
    extracompileargs.append("-std=c++11")
    extracompileargs.append("-std=c++14")

if sys.platform == "linux":
    include_dirs.append("/usr/include/eigen3")

if sys.platform == "darwin":
    include_dirs.append("/usr/local/include/eigen3")
    include_dirs.append("/opt/homebrew/include/eigen3")
    include_dirs.append("/opt/homebrew/include")
    
config.add_installed_library("cppsystemlib",
+cppsystemlib = ("cppsystemlib",
+                    {"sources": ['pysim/cppsource/CppSystem.cpp',
                     'pysim/cppsource/StoreHandler.cpp',
                     'pysim/cppsource/CommonSystemImpl.cpp',
                     'pysim/cppsource/Variable.cpp',
                     'pysim/cppsource/ConnectionHandler.cpp',
                     'pysim/cppsource/CompositeSystemImpl.cpp',
                     'pysim/cppsource/CppSimulation.cpp',
                    ],
                    "include_dirs": include_dirs})

                                
extensions = [Extension("pysim.cppsystem",
                        ['pysim/cppsystem.pyx',],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        libraries=["cppsystemlib",]
                        ),
              Extension("pysim.cythonsystem",
                        ['pysim/cythonsystem.pyx','pysim/cppsource/CythonSystemImpl.cpp'],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        libraries=["cppsystemlib",]
                        ),
              Extension("pysim.compositesystem",
                        ['pysim/compositesystem.pyx',],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        libraries=["cppsystemlib",]
                        ),
              Extension("pysim.simulatablesystem",
                        ['pysim/simulatablesystem.pyx'],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        libraries=["cppsystemlib",]
                        ),
              Extension("pysim.connections",
                        ['pysim/connections.pyx'],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        libraries=["cppsystemlib",]
                        ),
              Extension("pysim.commonsystem",
                        ['pysim/commonsystem.pyx'],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        libraries=["cppsystemlib",]
                        ),
              Extension("pysim.simulation",
                        ['pysim/simulation.pyx',],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        libraries=["cppsystemlib",]
                        ),
              Extension("pysim.systems.defaultsystemcollection1",
                         ['pysim/systems/defaultsystemcollection1/defaultsystemcollection1.pyx',
                          'pysim/systems/defaultsystemcollection1/cppsource/factory.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/MassSpringDamper.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/PredatorPrey.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/VanDerPol.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/Adder3D.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/Adder6D.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/Adder.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/ScalarAdder.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/SquareWave.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/DiscretePID.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/ReadTextInput.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/RigidBody.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/LogisticMap.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/ParameterTestSystem.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/InOutTestSystem.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/PostStepTestSystem.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/PreStepTestSystem.cpp',
                          ],
                          language="c++",
                          extra_compile_args=extracompileargs,
                          include_dirs=['pysim/systems/defaultsystemcollection1',],
                          libraries=["cppsystemlib",],
                          ),
             ]

def readme():
    with open('README.rst') as f:
        return f.read()

for e in extensions:
    e.cython_directives = {"embedsignature": True}
    e.include_dirs.extend(include_dirs)
    
setup(
    name="pysim",
    version=__version__,
    author="Linus Aldebjer",
    author_email="aldebjer@gmail.com",
    url="http://pysim.org",
    ext_modules=cythonize(extensions, include_path=["pysim"]),
    libraries=[cppsystemlib],
    cmdclass={'build_exe': BuildExeCommand, 
              'sdist': sdist},
    scripts=['scripts/new_pysim_system.py'],
    package_data={'pysim': ['*.pxd', 'cppsource/*.hpp', 'templates/*']},
    packages=['pysim', 'pysim.systems','pysim.tests'],
    install_requires=['colorama', 'jinja2', 'numpy>=1.8.1'],
    description = "Modeling and Simulation of Dynamical Systems",
    long_description=readme(),
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'Natural Language :: English',
        'License :: OSI Approved :: BSD License',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX :: Linux',
        'Operating System :: MacOS',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: Implementation :: CPython',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ],
)



