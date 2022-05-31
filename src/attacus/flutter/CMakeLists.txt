cmake_minimum_required(VERSION 3.4)

project(AttFlutter LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Sdl.cmake)
include(${ATT_ROOT}/cmake/Shell.cmake)
include(${ATT_ROOT}/cmake/Bgfx.cmake)
#include(${ATT_ROOT}/cmake/NanoVG.cmake)
include(${ATT_ROOT}/cmake/VgRenderer.cmake)


set(FLUTTER_HEADERS
    flutter_embedder.h
    flutter_view.h

    flutter_composite_view_vg.h
    flutter_composite_view_gfx.h

    flutter_composite_view_gl.h
    backing_store.h

    flutter_messenger.h
    flutter_runner.h

    channel.h
    method_channel.h
    standard_method_channel.h
    standard_message_codec.h
    standard_method_codec.h
    standard_codec_serializer.h

    engine_method_result.h
    method_codec.h
    method_call.h
    method_result.h
    encodable_value.h
    byte_streams.h
    byte_buffer_streams.h
    message_codec.h
    json.h
    json_method_channel.h
    json_message_codec.h
    json_method_codec.h

    components/flutter_component.h
    components/isolate.h
    components/platform.h
    components/navigation.h
    components/cursor.h
    components/mouse_input.h
    components/text_input.h
    components/text_input_model.h
    components/text_range.h
    components/string_conversion.h
    components/texture_registrar.h
    components/view_registrar.h

    compositor/vg/compositor_layer_vg.h
    compositor/vg/compositor_frame_vg.h
    compositor/vg/backing_layer_vg.h
    compositor/vg/backing_surface_vg.h
    compositor/vg/compositor_vg.h

    compositor/gfx/compositor_layer_gfx.h
    compositor/gfx/compositor_frame_gfx.h
    compositor/gfx/backing_layer_gfx.h
    compositor/gfx/backing_surface_gfx.h
    compositor/gfx/compositor_gfx.h

)

set(FLUTTER_SRC
    flutter_view.cpp

    flutter_composite_view_vg.cpp
    flutter_composite_view_gfx.cpp

    flutter_composite_view_gl.cpp
    backing_store.cpp
    
    flutter_messenger.cpp
    flutter_runner.cpp

    standard_codec.cpp
    json_message_codec.cpp
    json_method_codec.cpp

    components/flutter_component.cpp
    components/isolate.cpp
    components/platform.cpp
    components/navigation.cpp
    components/cursor.cpp
    components/mouse_input.cpp
    components/text_input.cpp
    components/text_input_model.cpp
    components/string_conversion.cpp
    components/texture_registrar.cpp
    components/view_registrar.cpp

    compositor/vg/compositor_layer_vg.cpp
    compositor/vg/compositor_frame_vg.cpp
    compositor/vg/backing_layer_vg.cpp
    compositor/vg/backing_surface_vg.cpp
    compositor/vg/compositor_vg.cpp

    compositor/gfx/compositor_layer_gfx.cpp
    compositor/gfx/compositor_frame_gfx.cpp
    compositor/gfx/backing_layer_gfx.cpp
    compositor/gfx/backing_surface_gfx.cpp
    compositor/gfx/compositor_gfx.cpp
)

add_library(${THIS} STATIC ${FLUTTER_SRC} ${FLUTTER_HEADERS})
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

USES_SDL(${THIS})
USES_BGFX(${THIS})
#USES_NVG(${THIS})
USES_VGR(${THIS})

if(${ATT_PLATFORM_WINDOWS})
add_library(FlutterEngine SHARED IMPORTED)
set_property(TARGET FlutterEngine PROPERTY
             IMPORTED_LOCATION ${ATT_ROOT}/binaries/flutter/windows-x64-embedder/flutter_engine.dll)
set_property(TARGET FlutterEngine PROPERTY
             IMPORTED_IMPLIB ${ATT_ROOT}/binaries/flutter/windows-x64-embedder/flutter_engine.dll.lib)
endif()

target_link_libraries(${THIS} PRIVATE FlutterEngine)
