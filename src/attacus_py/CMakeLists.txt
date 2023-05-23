cmake_minimum_required(VERSION 3.4)

set(THIS AttacusPy)
set(MODULE attucus)

project(${THIS} VERSION 0.1 LANGUAGES CXX)

include(${ATT_ROOT}/cmake/Pybind11.cmake)
include(${ATT_ROOT}/cmake/Attacus.cmake)

set(ATTPY_HEADERS
    bindings/bindtools.h
    bindings/conversions.h
)

set(ATTPY_SOURCES
    #bindings/generated.cpp
    bindings/main.cpp
)

pybind11_add_module(${THIS} MODULE
    bindings/init.cpp
)

set_target_properties(${THIS} PROPERTIES OUTPUT_NAME ${MODULE})

USES_PYBIND11(${THIS})
USES_ATT(${THIS})

target_sources(${THIS} PRIVATE ${ATTPY_SOURCES})
# add include files
target_sources(${THIS} PRIVATE ${ATTPY_HEADERS}) # for intellisense
target_include_directories(${THIS}
	PUBLIC
		$<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS ${THIS} LIBRARY DESTINATION ${ATT_ROOT}/${MODULE})
#set_target_properties(${THIS} PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY ${ATT_ROOT}/${MODULE})

set (FLUTTER_DLL ${ATT_ROOT}/binaries/windows-x64-embedder/flutter_engine.dll)
install(FILES ${FLUTTER_DLL} DESTINATION ${ATT_ROOT}/${MODULE})
#file(COPY ${FLUTTER_DLL} DESTINATION ${ATT_ROOT}/${MODULE})

# Get the FLUTTER_ROOT environment variable
set(FLUTTER_ROOT $ENV{FLUTTER_ROOT})

# Set the path to icudtl.dat
set(ICU_DATA ${FLUTTER_ROOT}/bin/cache/artifacts/engine/windows-x64/icudtl.dat)

# Install icudtl.dat to the same directory as your .pyd file
install(FILES ${ICU_DATA} DESTINATION ${ATT_ROOT}/${MODULE})
#file(COPY ${ICU_DATA} DESTINATION ${ATT_ROOT}/${MODULE})

#TODO: Getting: SKBuildInvalidFileInstallationError: CMake-installed files must be within the project root.