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

add_subdirectory(basic)
add_subdirectory(helloworld)
add_subdirectory(methodchannel)

add_subdirectory(flutter-cubes)

add_subdirectory(compositor-gl)
add_subdirectory(compositor-gl-cubes)
add_subdirectory(compositor-gl-view)

add_subdirectory(cubes)
add_subdirectory(nanovg)
