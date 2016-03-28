
cmake_minimum_required (VERSION 2.8.11)

project (rcm)

list(APPEND CMAKE_C_FLAGS "-std=c99 -Wall")
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wall")

set(LIBRARY_OUTPUT_PATH ${rcm_BINARY_DIR}/lib)
include_directories(${rcm_SOURCE_DIR}/include)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
