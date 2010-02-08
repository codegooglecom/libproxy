### Required
cmake_minimum_required(VERSION 2.6)
project(libproxy)


### Globals
# Set the version
set(CMAKE_PROJECT_VERSION 0.4.0)

# Enable testing
enable_testing()

# Setup our relative paths
if(${WIN32})
  set(rlibdir .)
  set(rmoduledir  modules/)
  set(rlibexecdir .)
  set(rbindir     .)
  set(rincludedir include/)
else()
  set(rlibdir lib${LIB_SUFFIX}/)
  set(rmoduledir  ${rlibdir}/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_VERSION}/modules/)
  set(rlibexecdir libexec/)
  set(rbindir     bin/)
  set(rincludedir include/)
endif()

# Setup our full paths
set(prefix     ${CMAKE_INSTALL_PREFIX})
set(libdir     ${prefix}/${rlibdir})
set(moduledir  ${prefix}/${rmoduledir})
set(libexecdir ${prefix}/${rlibexecdir})
set(bindir     ${prefix}/${rbindir})
set(includedir ${prefix}/${rincludedir})

# Add a global compilation stuff
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
else()
  add_definitions(-D_POSIX_C_SOURCE=1)
  set(CMAKE_CXX_FLAGS "-g -Wall -Werror -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
endif()

### Subdirectories
add_subdirectory(libmodman)
add_subdirectory(libproxy)
add_subdirectory(bindings)
add_subdirectory(utils)