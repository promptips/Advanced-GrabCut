cmake_minimum_required(VERSION 2.6)

PROJECT(GrabCut)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(CMakeHelpers)

# Compile with c++11 functionality.
if(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++11") # gcc >= 4.7
endif(UNIX)

#### Eigen ####
# Tell CMake to also look in the source directory to find some .cmake files
set(CMAKE_MODULE_PATH ${C