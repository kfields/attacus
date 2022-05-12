include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_FLUTTER THIS)
    USES_STD(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${FLUTTER_ROOT}/include
        #${FLUTTER_ROOT}/ephemeral
    )
    target_link_libraries(${THIS} PRIVATE AttFlutter)
endfunction()