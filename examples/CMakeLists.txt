cmake_minimum_required(VERSION 3.15)

project(AttExamples LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Attacus.cmake)

set(HEADERS
    example_app.h
)

set(SOURCES
    example_app.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})
USES_ATT(${THIS})

add_subdirectory(00-helloworld)
add_subdirectory(01-methodchannel)
add_subdirectory(02-texture)
