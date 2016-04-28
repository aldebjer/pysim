__copyright__ = 'Copyright (c) 2014-2016 SSPA Sweden AB'

import setuptools
from numpy.distutils.core import setup
from numpy.distutils.misc_util import Configuration
from distutils.extension import Extension
from Cython.Build import cythonize
import sys
import numpy
import os

config = Configuration()
config.add_include_dirs(['pysim/cppsource', numpy.get_include()])

extracompileargs = []
if sys.platform == "win32":
    config.add_include_dirs([os.environ.get('BOOST_ROOT')])
        
elif sys.platform == "linux":
    extracompileargs.append("-std=c++11")

config.add_installed_library("cppsystemlib",
                    ['pysim/cppsource/CppSystem.cpp',],build_info = {
                    "extra_compiler_args":extracompileargs,
                    "language":"c++"},
                    install_dir = "pysim/lib",
                    )

                                
extensions = [Extension("pysim.cppsystem",
                        ['pysim/cppsystem.pyx',],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        libraries=["cppsystemlib",]
                        ),
              Extension("pysim.simulation",
                        ['pysim/simulation.pyx', 'pysim/cppsource/CppSimulation.cpp'],
                        language="c++",
                        extra_compile_args=extracompileargs,
                        ),
              Extension("pysim.systems.defaultsystemcollection1",
                         ['pysim/systems/defaultsystemcollection1/defaultsystemcollection1.pyx',
                          'pysim/systems/defaultsystemcollection1/cppsource/Factory.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/MassSpringDamper.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/PredatorPrey.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/VanDerPolSystem.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/Adder3D.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/ScalarAdder.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/SquareWave.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/DiscretePID.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/ReadTextInput.cpp',
                          'pysim/systems/defaultsystemcollection1/cppsource/RigidBody.cpp',
                          ],
                          language="c++",
                          extra_compile_args=extracompileargs,
                          include_dirs=['pysim/systems/defaultsystemcollection1',],
                          libraries=["cppsystemlib",],
                          ),
             ]
             
setup(
    name="pysim",
    version="2.0.dev0",
    author="Linus Aldebjer",
    author_email="linus.aldebjer@sspa.se",
    url="http://www.sspa.se",
    ext_modules=cythonize(extensions),

    #todo: this is windows specific, would be better to find the path to the built lib in some way
    data_files=[('pysim/include',['pysim/cppsource/simulatablesystem.hpp',
                                                    'pysim/cppsource/cppsystem.hpp']),
                ('pysim/lib',['build/temp.win32-{}/cppsystemlib.lib'.format(sys.version[:3])]),
                ('pysim',['pysim/cppsystem.pxd']),
                ],
    packages=['pysim', 'pysim.systems','pysim.tests'],
    install_requires = ['numpy>=1.8.1','cython>=0.20'],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Natural Language :: English',
        'Operating System :: Microsoft :: Windows',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: Implementation :: CPython',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ],
    **config.todict()
)
