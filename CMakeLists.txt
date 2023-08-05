cmake_minimum_required(VERSION 3.24)
project(pinepp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -DNDEBUG -O3 -g2")
include(FetchContent)
include(GoogleTest)

FETCHCONTENT_DECLARE(
        googletest
        URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)
FETCHCONTENT_MAKEAVAILABLE(googletest)

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_library(pinepp SHARED
        ${CMAKE_SOURCE_DIR}/inc/base64.hpp
        ${CMAKE_SOURCE_DIR}/src/base64.cpp
        ${CMAKE_SOURCE_DIR}/inc/bit_pattern.hpp
        ${CMAKE_SOURCE_DIR}/src/bit_pattern.cpp
        ${CMAKE_SOURCE_DIR}/inc/utility.hpp
        ${CMAKE_SOURCE_DIR}/src/utility.cpp
        ${CMAKE_SOURCE_DIR}/inc/concepts.hpp
        ${CMAKE_SOURCE_DIR}/inc/trie.hpp)

enable_testing()
add_executable(trie_test ${CMAKE_SOURCE_DIR}/test/trie.test.cpp)
target_link_libraries(trie_test gtest_main pinepp)
ADD_TEST(NAME trie COMMAND trie_test)

add_executable(utility_test ${CMAKE_SOURCE_DIR}/test/utility.test.cpp)
target_link_libraries(utility_test gtest_main pinepp)
ADD_TEST(NAME utility COMMAND utility_test)

add_executable(concepts_test ${CMAKE_SOURCE_DIR}/test/concepts.test.cpp)
target_link_libraries(concepts_test gtest_main pinepp)
ADD_TEST(NAME concepts COMMAND concepts_test)

add_executable(bit_pattern_test ${CMAKE_SOURCE_DIR}/test/bit_pattern.test.cpp)
target_link_libraries(bit_pattern_test gtest_main pinepp)
ADD_TEST(NAME bit_pattern COMMAND bit_pattern_test)