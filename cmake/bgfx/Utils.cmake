include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/../Standard.cmake)

function(USES_BGFX_UTILS THIS)
  USES_STD(${THIS})
  target_link_libraries(${THIS} PRIVATE BgfxUtils)

endfunction()
