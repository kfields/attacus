cmake_minimum_required(VERSION 3.15)

project(AttacusPy VERSION 0.1 LANGUAGES CXX)

#if(SKBUILD)
#  execute_process(
#    COMMAND "${PYTHON_EXECUTABLE}" -c
#            "import pybind11; print(pybind11.get_cmake_dir())"
#    OUTPUT_VARIABLE _tmp_dir
#    OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
#  list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
#endif()

# Now we can find pybind11
#find_package(pybind11 CONFIG REQUIRED)

set(THIS ${PROJECT_NAME})

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

set_target_properties(${THIS} PROPERTIES OUTPUT_NAME "attacus_py")

USES_PYBIND11(${THIS})
USES_ATT(${THIS})

#target_compile_definitions(${THIS} PRIVATE ATT_DLL)

#

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
#
# INSTALL
#
# install headers
#install(FILES ${IMGUI_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS ${THIS} SDL2 DESTINATION ${CMAKE_INSTALL_FULL_BINDIR} EXPORT ${THIS}Targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Quiet a warning, since this project is only valid with SKBUILD
set(ignoreMe "${SKBUILD}")

#ADD_CUSTOM_COMMAND(TARGET attacus
#          POST_BUILD
#          #COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}
#          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_INSTALL_LIBDIR}/libattacus.so ./
#)

#
# Generate a configuration for use by plugins
#
message(Build ${BUILD_DIR})
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${BUILD_DIR}/${THIS}/${THIS}ConfigVersion.cmake"
  VERSION ${Upstream_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT ${THIS}Targets
  FILE "${BUILD_DIR}/${THIS}/${THIS}Targets.cmake"
  NAMESPACE Upstream::
)
configure_file(cmake/${THIS}Config.cmake
  "${BUILD_DIR}/${THIS}/${THIS}Config.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/${THIS})
install(EXPORT ${THIS}Targets
  FILE
  ${THIS}Targets.cmake
  NAMESPACE
    Upstream::
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    cmake/${THIS}Config.cmake
    "${BUILD_DIR}/${THIS}/${THIS}Config.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)