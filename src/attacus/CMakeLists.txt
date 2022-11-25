cmake_minimum_required(VERSION 3.4)

project(Attacus LANGUAGES CXX)

set(THIS ${PROJECT_NAME})


include(../../cmake/Core.cmake)
include(../../cmake/Shell.cmake)
include(../../cmake/Flutter.cmake)

set(ATT_HEADERS
    app.h
)

set(ATT_SOURCES
    app.cpp
)

if(${HAS_APP})

add_library(${THIS} STATIC ${ATT_SOURCES} ${ATT_HEADERS})
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

USES_CORE(${THIS})
USES_SHELL(${THIS})
USES_FLUTTER(${THIS})

endif() #HAS_APP

add_subdirectory(core)

if(${HAS_SHELL})
add_subdirectory(shell)
endif() #HAS_SHELL

if(${HAS_FLUTTER})
add_subdirectory(flutter)
endif() #HAS_FLUTTER