cmake_minimum_required(VERSION 3.11)

project(calc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(calc ${SRCS})