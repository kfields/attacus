cmake_minimum_required(VERSION 3.4)

project(cubes_view LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/ExampleViews.cmake)

set(HEADERS
    cubes_view.h
)

set(SOURCES
    cubes_view.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})
USES_EXAMPLE_VIEWS(${THIS})
