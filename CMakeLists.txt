cmake_minimum_required (VERSION 2.8)

project(react)

set(react_VERSION 0.0.1)

option(DEBUG "Enables assertions and logging" ON)
option(DEMOS "Compile demo application" OFF)
option(TESTS "Compile tests specs (requires gtest library)" OFF)

if(DEBUG)
  add_definitions(-g -DNDEBUG)
endif(DEBUG)
add_definitions(-std=c++11 -Wall -Wextra)

include_directories(
  ${react_SOURCE_DIR}
  ${react_SOURCE_DIR}/include
)

add_subdirectory(src)
if(TESTS)
  add_subdirectory(tests)
endif(TESTS)
if(DEMOS)
  add_subdirectory(demos)
endif(DEMOS)

