cmake_minimum_required(VERSION 3.14)

include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Config.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Fmt.cmake)

function(USES_STD THIS)
    USES_FMT(${THIS})

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
    #target_compile_features(${THIS} PUBLIC cxx_std_17)
    target_compile_features(${THIS} PUBLIC cxx_std_20)
    
    target_include_directories(${THIS} PRIVATE
        ${ATT_ROOT}/src
    )
endfunction()

