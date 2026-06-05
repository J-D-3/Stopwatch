cmake_minimum_required(VERSION 3.14)
project(Stopwatch LANGUAGES CXX)

# Header-only library. Consumers just link `stopwatch` to get the include
# path and the C++11 requirement; there is nothing to compile.
add_library(stopwatch INTERFACE)
target_include_directories(stopwatch INTERFACE
   ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(stopwatch INTERFACE cxx_std_11)

# Only build the demo and tests when Stopwatch is the top-level project,
# so it stays a drop-in dependency when added via add_subdirectory().
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
   enable_testing()

   add_executable(stopwatch_demo test/main.cpp)
   target_link_libraries(stopwatch_demo PRIVATE stopwatch)

   add_executable(stopwatch_tests test/test_stopwatch.cpp test/test_link_tu.cpp)
   target_link_libraries(stopwatch_tests PRIVATE stopwatch)
   add_test(NAME stopwatch_tests COMMAND stopwatch_tests)
endif()
