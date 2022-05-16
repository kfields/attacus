#include <iostream>

#include <attacus/flutter/encodable_value.h>
#include <attacus/flutter/flutter_window.h>
#include <attacus/flutter/standard_method_channel.h>

#include "cursor.h"

namespace attacus {

static constexpr char kChannelName[] = "flutter/mousecursor";

static constexpr char kActivateSystemCursorMethod[] = "activateSystemCursor";

static constexpr char kKindKey[] = "kind";

CursorComponent::CursorComponent(FlutterWindow &view) : FlutterComponent(view)
{
}

void CursorComponent::Create()
{
    // Cursors
    cursor_arrow_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    cursor_ibeam_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    cursor_wait_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
    cursor_crosshair_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    cursor_waitarrow_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
    cursor_size_nwse_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    cursor_size_nesw_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    cursor_size_we_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    cursor_size_ns_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    cursor_size_all_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    cursor_no_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
    cursor_hand_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    delegates_ = {
        { "basic", [this](){ SDL_SetCursor(cursor_arrow_);} },
        { "click", [this](){ SDL_SetCursor(cursor_hand_);} },
        { "text",  [this](){ SDL_SetCursor(cursor_ibeam_);} },
    };

    FlutterMessenger &messenger = flutter().messenger();
    channel_ = new StandardMethodChannel(messenger, kChannelName);
    channel()
        .def(kActivateSystemCursorMethod,
            [this](const StandardMethodCall& call, std::unique_ptr<StandardMethodResult> result)
            {
                const auto& arguments = std::get<EncodableMap>(*call.arguments());
                auto kind_iter = arguments.find(EncodableValue(std::string(kKindKey)));
                if (kind_iter == arguments.end()) {
                result->Error("Argument error",
                                "Missing argument while trying to activate system cursor");
                return;
                }
                const auto& kind = std::get<std::string>(kind_iter->second);
                std::cout << "activateSystemCursor: "  << kind << std::endl;
                delegates_[kind]();
                result->Success();
            });

}

} // namespace attacus