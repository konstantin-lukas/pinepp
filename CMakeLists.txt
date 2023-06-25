cmake_minimum_required(VERSION 3.24)
project(pine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "-Wall -Werror -DNDEBUG -O3 -g2")

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_library(pinepp SHARED
        ${CMAKE_SOURCE_DIR}/src/base64.cpp
        ${CMAKE_SOURCE_DIR}/src/bit_pattern.cpp
        ${CMAKE_SOURCE_DIR}/inc/base64.hpp
        ${CMAKE_SOURCE_DIR}/inc/bit_pattern.hpp
        ${CMAKE_SOURCE_DIR}/inc/concepts.hpp
        ${CMAKE_SOURCE_DIR}/inc/trie.hpp)

add_executable(main ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(main PUBLIC pinepp)