#pragma once

#include "flutter_component.h"

#include "text_input_model.h"

namespace attacus {

class FlutterView;
class JsonMethodChannel;
class TextInputModel;

class TextInput : FlutterComponent {
public:
    TextInput(FlutterView& view);
    void Create();
    void SendStateUpdate(const TextInputModel& model);
    bool Dispatch(SDL_Event &e) override;
    void DispatchKey(const SDL_KeyboardEvent &event);
    void DispatchText(const SDL_TextInputEvent &event);
    void EnterPressed(TextInputModel* model);
    // Accessors
    JsonMethodChannel& channel() { return *channel_; }
    // Data members
    JsonMethodChannel* channel_ = nullptr;
    // The active model. nullptr if not set.
    std::unique_ptr<TextInputModel> active_model_;
    // The active client id.
    int client_id_ = 0;

    // Keyboard type of the client. See available options:
    // https://api.flutter.dev/flutter/services/TextInputType-class.html
    std::string input_type_;

    // An action requested by the user on the input client. See available options:
    // https://api.flutter.dev/flutter/services/TextInputAction-class.html
    std::string input_action_;

};

} //namespace attacus