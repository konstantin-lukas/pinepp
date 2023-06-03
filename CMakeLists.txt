cmake_minimum_required(VERSION 3.24)
project(pine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_CXX_FLAGS "-Wall -Werror")

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_library(base64 SHARED
        ${CMAKE_SOURCE_DIR}/src/base64.cpp
        ${CMAKE_SOURCE_DIR}/inc/base64.hpp)
add_library(bit_pattern SHARED
        ${CMAKE_SOURCE_DIR}/src/bit_pattern.cpp
        ${CMAKE_SOURCE_DIR}/inc/bit_pattern.hpp)

add_executable(main ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(main PUBLIC bit_pattern base64)