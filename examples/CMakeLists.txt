cmake_minimum_required(VERSION 3.4)

project(AttExamples LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Attacus.cmake)
include(${ATT_ROOT}/cmake/Bgfx.cmake)

set(HEADERS
    example_app.h
)

set(SOURCES
    example_app.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})
USES_ATT(${THIS})
USES_BGFX(${THIS})

add_subdirectory(00-helloworld)
add_subdirectory(01-methodchannel)
add_subdirectory(10-cubes)
add_subdirectory(11-flutter-cubes)
add_subdirectory(12-compositor)
add_subdirectory(20-nanovg)
