cmake_minimum_required(VERSION 3.4)

project(120-nanovg LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Examples.cmake)
include(${ATT_ROOT}/cmake/NanoVG.cmake)

set(HEADERS
    main.h
)

set(SOURCES
    main.cpp
    data.cpp
    util.cpp
    window.cpp
    searchbox.cpp
    eyes.cpp
    paragraph.cpp
    lines.cpp
    graph.cpp
    colorwheel.cpp
    scissor.cpp
)

add_executable(${THIS} ${SOURCES} ${HEADERS})
USES_EXAMPLES(${THIS})
USES_NVG(${THIS})
