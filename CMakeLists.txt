cmake_minimum_required (VERSION 2.8)
project (blif)

set (CMAKE_CXX_FLAGS "-pipe -std=c++11 -Wall -Wextra -march=native")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -D_DEBUG")
set (CMAKE_CXX_FLAGS_RELEASE "-O2 -flto -DNDEBUG")

find_package (Boost REQUIRED COMPONENTS unit_test_framework)
enable_testing ()
macro (add_unittest NAME MAIN_SRC)
    add_executable (${NAME} ${MAIN_SRC} ${ARGN})
    target_link_libraries (${NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    add_test (${NAME} ${NAME})
endmacro (add_unittest)

include_directories (src)
add_subdirectory (src)
