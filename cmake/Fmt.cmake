include_guard()

function(USES_FMT THIS)
  target_include_directories(${THIS} PRIVATE
    ${FMT_ROOT/include}
  )
  target_link_libraries(${THIS} PRIVATE fmt)
endfunction()
