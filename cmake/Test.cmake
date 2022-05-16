cmake_minimum_required(VERSION 3.14)

include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_TEST THIS)
    USES_STD(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${GOOGLE_TEST_ROOT}/googletest/include
    )
    target_link_libraries(${THIS} PRIVATE gtest_main)

endfunction()

