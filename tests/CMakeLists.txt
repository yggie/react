cmake_minimum_required(VERSION 2.6)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(".")

add_executable(tests main.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread react)

add_custom_target(
  run_all_tests ALL
)

add_custom_command(
  TARGET run_all_tests
  COMMENT "Running all tests"
  POST_BUILD COMMAND tests
)

add_subdirectory(Math)
add_subdirectory(Collision/Shapes)
add_subdirectory(Entities)
add_subdirectory(Utilities)
add_subdirectory(Collision)
add_subdirectory(Integration)

