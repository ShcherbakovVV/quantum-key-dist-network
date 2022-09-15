cmake_minimum_required(VERSION 3.20.0)

project(qkd VERSION 1.0.0 LANGUAGES CXX)

add_compile_options(--std=c++20 -O3 -Wall -Wpedantic -Wconversion -Wextra)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories("${CMAKE_SOURCE_DIR}/qkd/include")

enable_testing()

add_subdirectory("${CMAKE_SOURCE_DIR}/qkd")
add_subdirectory("${CMAKE_SOURCE_DIR}/tests")
