project(
    'pysim',
    'cpp',
    'cython',
    default_options : [
        'cpp_std=c++14',
        ],
)

py_mod = import('python')
py = py_mod.find_installation(pure: false)

cy = meson.get_compiler('cython')
cxx = meson.get_compiler('cpp')

# Platform-specific Boost dependency handling
# Boost is header-only in this project (ublas, odeint, algorithm, format, variant, property_tree)
if host_machine.system() == 'windows'
  # On Windows, use BOOST_ROOT environment variable to locate headers
  boost_root = run_command('python', '-c',
    'import os; print(os.environ.get("BOOST_ROOT", ""))',
    check: false).stdout().strip()

  if boost_root != ''
    # Create a header-only dependency using include_directories
    boost_inc = include_directories(boost_root)
    boost_dep = declare_dependency(
      include_directories: boost_inc,
      version: '1.90.0',  # Match the version downloaded in CI
      compile_args: ['-DBOOST_ALL_NO_LIB']  # Disable MSVC auto-linking
    )
  else
    # Fallback to standard dependency detection if BOOST_ROOT not set
    boost_dep = dependency('boost', required: false)
    if not boost_dep.found()
      error('Boost not found. On Windows, please set BOOST_ROOT environment variable.')
    endif
  endif
else
  # On Linux/macOS, use standard pkg-config based detection
  boost_dep = dependency('boost', required: true)
endif

eigen_dep = dependency('eigen3', required: true)
numpy_dep = dependency('numpy', required: true)

base_cython_args = []
if cy.version().version_compare('>=3.1.0')
  base_cython_args += ['-Xfreethreading_compatible=True']
endif

cython_cpp_lib = static_library(
    'cppsystemlib',
    'pysim/cppsource/CppSystem.cpp',
    'pysim/cppsource/StoreHandler.cpp',   
    'pysim/cppsource/CommonSystemImpl.cpp',
    'pysim/cppsource/Variable.cpp',
    'pysim/cppsource/ConnectionHandler.cpp',
    'pysim/cppsource/CompositeSystemImpl.cpp',
    'pysim/cppsource/CppSimulation.cpp',
   
    dependencies : [boost_dep, eigen_dep],
    include_directories :['pysim/cppsource'],
    install: true,
    install_dir: py.get_install_dir() / 'pysim/lib',
)

install_data(
    [
        'pysim/cppsystem.pxd',
        'pysim/commonsystem.pxd',
        'pysim/simulatablesystem.pxd',
        'pysim/cythonsystem.pxd',
        'pysim/connections.pxd',
        'pysim/compositesystem.pxd',
        ],
    install_dir: py.get_install_dir() / 'pysim',
)

install_data(
    ['pysim/cppsource/SimulatableSystem.hpp',
        'pysim/cppsource/CppSystem.hpp',
        'pysim/cppsource/PysimTypes.hpp',
        'pysim/cppsource/CommonSystemImpl.hpp',
        'pysim/cppsource/Variable.hpp',
        'pysim/cppsource/StoreHandler.hpp',
        'pysim/cppsource/CythonSystemImpl.hpp',
        'pysim/cppsource/ConnectionHandler.hpp',
        'pysim/cppsource/CompositeSystemImpl.hpp',
        ],
    install_dir: py.get_install_dir() / 'pysim/include',
)


py.install_sources(
    [
    'pysim/__init__.py',
    'pysim/info.py',
    ],
    subdir: 'pysim',
)

prog_cython = find_program('cython', required: true)

cythonsystem_api_h = custom_target(
    'cythonsystem_api.h',
    input: 'pysim/cythonsystem.pyx',
    output: 'cythonsystem_api.h',
    command: [prog_cython, '@INPUT@', '--fast-fail', '--cplus', '-Xfreethreading_compatible=True'])

cppsystem = py.extension_module(
    'cppsystem',
    'pysim/cppsystem.pyx',
    link_with: [cython_cpp_lib],
    cython_args: base_cython_args,
    override_options: ['cython_language=cpp'],
    dependencies : [boost_dep, eigen_dep, numpy_dep],
    include_directories :['pysim/cppsource'],
    install: true,
    subdir: 'pysim',
)

py.extension_module(
    'cythonsystem',
    ['pysim/cythonsystem.pyx','pysim/cppsource/CythonSystemImpl.cpp', cythonsystem_api_h],
    link_with: [cython_cpp_lib],
    cython_args: base_cython_args,
    override_options: ['cython_language=cpp'],
    dependencies : [boost_dep, eigen_dep, numpy_dep],
    include_directories :['pysim/cppsource'],
    install: true,
    subdir: 'pysim',
)

py.extension_module(
    'compositesystem',
    'pysim/compositesystem.pyx',
    link_with: [cython_cpp_lib],
    cython_args: base_cython_args,
    override_options: ['cython_language=cpp'],
    dependencies : [boost_dep, eigen_dep, numpy_dep],
    include_directories :['pysim/cppsource'],
    install: true,
    subdir: 'pysim',
)

py.extension_module(
    'simulatablesystem',
    'pysim/simulatablesystem.pyx',
    link_with: [cython_cpp_lib],
    cython_args: base_cython_args,
    override_options: ['cython_language=cpp'],
    dependencies : [boost_dep, eigen_dep, numpy_dep],
    include_directories :['pysim/cppsource'],
    install: true,
    subdir: 'pysim',
)

py.extension_module(
    'connections',
    'pysim/connections.pyx',
    link_with: [cython_cpp_lib],
    cython_args: base_cython_args,
    override_options: ['cython_language=cpp'],
    dependencies : [boost_dep, eigen_dep, numpy_dep],
    include_directories :['pysim/cppsource'],
    install: true,
    subdir: 'pysim',
)

py.extension_module(
    'commonsystem',
    'pysim/commonsystem.pyx',
    link_with: [cython_cpp_lib],
    cython_args: base_cython_args,
    override_options: ['cython_language=cpp'],
    dependencies : [boost_dep, eigen_dep, numpy_dep],
    include_directories :['pysim/cppsource'],
    install: true,
    subdir: 'pysim',
)

py.extension_module(
    'simulation',
    'pysim/simulation.pyx',
    link_with: [cython_cpp_lib],
    cython_args: [base_cython_args],
    override_options: ['cython_language=cpp'],
    dependencies : [boost_dep, eigen_dep, numpy_dep],
    include_directories :['pysim/cppsource'],
    install: true,
    subdir: 'pysim',
)

py.install_sources(
    ['pysim/systems/__init__.py',
    'pysim/systems/python_systems.py',
    ],
    subdir: 'pysim/systems',
)

py.extension_module(
    'defaultsystemcollection1',
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
    link_with: [cython_cpp_lib],
    cython_args: ['-I', meson.current_source_dir()] + base_cython_args,
    dependencies : [boost_dep, eigen_dep, numpy_dep],
    override_options: ['cython_language=cpp'],
    include_directories :['pysim/cppsource', 'pysim/systems/defaultsystemcollection1'],
    install: true,
    subdir: 'pysim/systems',
)

py.install_sources(
    [
        'pysim/tests/__init__.py',
        'pysim/tests/compositesystem_test.py',
        'pysim/tests/configfile_test.py',
        'pysim/tests/cppsystem_test.py',
        'pysim/tests/input_ouput_test.py',
        'pysim/tests/integration_test.py',
        'pysim/tests/parameter_test.py',
        'pysim/tests/poststep_test.py',
        'pysim/tests/prestep_test.py',
        'pysim/tests/simulation_test.py',
        'pysim/tests/state_test.py',
        'pysim/tests/store_test.py',
        'pysim/tests/system_readtextinput_test.py',
    ],
    subdir: 'pysim/tests',
)