cmake_minimum_required(VERSION 3.8.0)
project(MusicPlayerDemo VERSION 0.0.1)

message("Start building...")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_definitions(
    #-DPLATFORM_WINDOWS   # define marco
)

# common compiling options
add_compile_options(
    -Wl,--no-as-needed
    -fno-strict-aliasing
    -fthreadsafe-statics
    -pthread
    #-fstack-protector-strong
    -fno-short-enums
    -fPIC
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})
message(STATUS "ENABLE_VULKAN:" ${ENABLE_VULKAN})

# SOURCE SET
set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR}/bin)

add_subdirectory(src)