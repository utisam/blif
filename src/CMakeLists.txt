cmake_minimum_required (VERSION 2.8)

set (NOMAIN_SOURCES
    circuit.cpp
    gate.cpp
    parse.cpp
    remodeler.cpp
    trimmed_lines.cpp
)
add_library (${CMAKE_PROJECT_NAME} SHARED ${NOMAIN_SOURCES})

add_unittest (unittest unittest.cpp ${NOMAIN_SOURCES})

install (TARGETS ${CMAKE_PROJECT_NAME} LIBRARY DESTINATION lib64)
install (DIRECTORY . DESTINATION "include"
         FILES_MATCHING PATTERN "*.h"
         PATTERN "doc" EXCLUDE)
