cmake_minimum_required(VERSION 3.14)

set(THIS ${PROJECT_NAME}${CMAKE_SYSTEM_NAME})

include(${ATT_ROOT}/cmake/Sdl.cmake)
include(${ATT_ROOT}/cmake/Flutter.cmake)
include(${ATT_ROOT}/cmake/Bgfx.cmake)

file(GLOB SHELL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SHELL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(${THIS} STATIC
  ${SHELL_SRC}
  ${SHELL_INCLUDE}
)
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

USES_SDL(${THIS})
USES_FLUTTER(${THIS})
USES_BGFX(${THIS})