cmake_minimum_required(VERSION 3.14)

include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Config.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Fmt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Glm.cmake)

function(USES_STD THIS)
    USES_FMT(${THIS})
    USES_GLM(${THIS})

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${THIS} PRIVATE 
            ${ATT_COMPILE_DEFS}
            BX_CONFIG_DEBUG=1
        )
    else()
        target_compile_definitions(${THIS} PRIVATE
            ${ATT_COMPILE_DEFS}
            BX_CONFIG_DEBUG=0
        )
    endif()

    if(${ATT_PLATFORM} STREQUAL "Linux")
        target_compile_definitions(${THIS} PRIVATE
            ${ATT_COMPILE_DEFS}
            NOMINMAX
        )
    endif()

    target_compile_features(${THIS} PUBLIC cxx_std_20)
    set_property(TARGET ${THIS} PROPERTY CXX_STANDARD 20)
    if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
        target_compile_options(${THIS} PRIVATE "/Zc:__cplusplus")
    endif()

    target_include_directories(${THIS} PRIVATE
        ${ATT_ROOT}/src
    )
endfunction()

