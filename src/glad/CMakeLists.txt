cmake_minimum_required(VERSION 3.4)

project(Glad LANGUAGES CXX)

set(THIS ${PROJECT_NAME})
set(_ ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GLAD_HEADERS
    ${_}/include/glad/gl.h
    ${_}/include/KHR/khrplatform.h
)

set(GLAD_SRC
    ${_}/src/gl.c
)

add_library(${THIS} STATIC ${GLAD_SRC} ${GLAD_HEADERS})
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

target_include_directories(${THIS} PRIVATE
    ${_}/include
)
