#pragma once

#include <map>

#include <attacus/flutter/flutter_embedder.h>

#include "flutter_component.h"

namespace attacus {

class FlutterView;
class StandardMethodChannel;
class TextInputModel;

class CursorComponent : FlutterComponent {
public:
    CursorComponent(FlutterView& view);
    void Create();
    // Accessors
    StandardMethodChannel& channel() { return *channel_; }
    // Data members
    StandardMethodChannel* channel_ = nullptr;
    std::map<std::string, Delegate> delegates_;
    //
    SDL_Cursor* cursor_arrow_; //SDL_SYSTEM_CURSOR_ARROW
    SDL_Cursor* cursor_ibeam_; //SDL_SYSTEM_CURSOR_IBEAM
    SDL_Cursor* cursor_wait_; //SDL_SYSTEM_CURSOR_WAIT
    SDL_Cursor* cursor_crosshair_; //SDL_SYSTEM_CURSOR_CROSSHAIR
    SDL_Cursor* cursor_waitarrow_; //SDL_SYSTEM_CURSOR_WAITARROW
    SDL_Cursor* cursor_size_nwse_; //SDL_SYSTEM_CURSOR_SIZENWSE
    SDL_Cursor* cursor_size_nesw_; //SDL_SYSTEM_CURSOR_SIZENESW
    SDL_Cursor* cursor_size_we_; //SDL_SYSTEM_CURSOR_SIZEWE
    SDL_Cursor* cursor_size_ns_; //SDL_SYSTEM_CURSOR_SIZENS
    SDL_Cursor* cursor_size_all_; //SDL_SYSTEM_CURSOR_SIZEALL
    SDL_Cursor* cursor_no_; //SDL_SYSTEM_CURSOR_NO
    SDL_Cursor* cursor_hand_; //SDL_SYSTEM_CURSOR_HAND
};

} //namespace attacus