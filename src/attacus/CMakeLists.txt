cmake_minimum_required(VERSION 3.4)

project(Attacus LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(../../cmake/Shell.cmake)
include(../../cmake/Flutter.cmake)

set(ATT_HEADERS
    app.h
)

set(ATT_SOURCES
    app.cpp
)

add_library(${THIS} STATIC ${ATT_SOURCES} ${ATT_HEADERS})
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

USES_SHELL(${THIS})
USES_FLUTTER(${THIS})

add_subdirectory(shell)
add_subdirectory(flutter)
