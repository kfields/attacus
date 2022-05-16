cmake_minimum_required(VERSION 3.14)

include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_RXCPP THIS)
    USES_STD(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${RXCPP_ROOT}/Rx/v2/src
    )
endfunction()

