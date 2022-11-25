include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_FLUTTER THIS)
    USES_STD(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${FLUTTER_OUT}
    )
    target_precompile_headers(${THIS} PRIVATE ${FLUTTER_OUT}/flutter_embedder.h)
    target_link_libraries(${THIS} PRIVATE AttFlutter)
endfunction()