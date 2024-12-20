cmake_minimum_required(VERSION 3.15)
project(enigma VERSION 0.0.1)

message("Start building...")

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(LIB_NAME "${PROJECT_NAME}")
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SRC_DIR "${ROOT_DIR}/enigma")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT_DIR}/output")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ROOT_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/output")

message(STATUS "CMake is using the following C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CMake is using the following C++ compiler: ${CMAKE_CXX_COMPILER}")

add_subdirectory(enigma)
