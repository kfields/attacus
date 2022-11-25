include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_SOKOL THIS)
  USES_STD(${THIS})
  target_include_directories(${THIS} PRIVATE
    ${SOKOL_ROOT}
  )
endfunction()
