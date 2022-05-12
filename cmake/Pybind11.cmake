include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

#TODO:Need CMake 3.20 to use cmake_path
#cmake_path(SET BUILD_DIR "$ENV{VIRTUAL_ENV}/Build/cmake")
file(TO_CMAKE_PATH "$ENV{VIRTUAL_ENV}/Build/cmake" BUILD_DIR)
list(APPEND CMAKE_PREFIX_PATH ${BUILD_DIR})

function(USES_PYBIND11 THIS)
    USES_STD(${THIS})
    #set_target_properties(${project} PROPERTIES PREFIX "lib")

    if(CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${THIS} PROPERTIES SUFFIX ".so")
    else()
        set_target_properties(${THIS} PROPERTIES SUFFIX ".pyd")
    endif(CMAKE_COMPILER_IS_GNUCXX)

    #target_include_directories(${THIS} PRIVATE
    #    ${PYBIND11_ROOT}/include
    #)

    #find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
    #find_package(pybind11 CONFIG)
    #include_directories(${Python3_INCLUDE_DIRS})
endfunction()