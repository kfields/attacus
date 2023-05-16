
#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/json_method_channel.h>

#include "text_input.h"

static constexpr char kSetEditingStateMethod[] = "TextInput.setEditingState";
static constexpr char kClearClientMethod[] = "TextInput.clearClient";
static constexpr char kSetClientMethod[] = "TextInput.setClient";
static constexpr char kShowMethod[] = "TextInput.show";
static constexpr char kHideMethod[] = "TextInput.hide";

static constexpr char kMultilineInputType[] = "TextInputType.multiline";

static constexpr char kUpdateEditingStateMethod[] =
    "TextInputClient.updateEditingState";
static constexpr char kPerformActionMethod[] = "TextInputClient.performAction";

static constexpr char kTextInputAction[] = "inputAction";
static constexpr char kTextInputType[] = "inputType";
static constexpr char kTextInputTypeName[] = "name";
static constexpr char kComposingBaseKey[] = "composingBase";
static constexpr char kComposingExtentKey[] = "composingExtent";
static constexpr char kSelectionAffinityKey[] = "selectionAffinity";
static constexpr char kAffinityDownstream[] = "TextAffinity.downstream";
static constexpr char kSelectionBaseKey[] = "selectionBase";
static constexpr char kSelectionExtentKey[] = "selectionExtent";
static constexpr char kSelectionIsDirectionalKey[] = "selectionIsDirectional";
static constexpr char kTextKey[] = "text";

static constexpr char kChannelName[] = "flutter/textinput";

static constexpr char kBadArgumentError[] = "Bad Arguments";
static constexpr char kInternalConsistencyError[] =
    "Internal Consistency Error";

namespace attacus
{

TextInput::TextInput(FlutterView &view) : FlutterComponent(view)
{
}

void TextInput::Create()
{
    FlutterMessenger &messenger = flutter().messenger();

    channel_ = new JsonMethodChannel(messenger, kChannelName);

    channel()
    .def(kShowMethod,
        [this](const JsonMethodCall &call, std::unique_ptr<JsonMethodResult> result)
        {
            result->Success();
        })
    .def(kHideMethod,
        [this](const JsonMethodCall &call, std::unique_ptr<JsonMethodResult> result)
        {
            result->Success();
        })
    .def(kClearClientMethod,
        [this](const JsonMethodCall &call, std::unique_ptr<JsonMethodResult> result)
        {
            active_model_ = nullptr;
            result->Success();
        })
    .def(kSetClientMethod,
        [this](const JsonMethodCall &call, std::unique_ptr<JsonMethodResult> result)
        {
            if (!call.arguments() || call.arguments()->is_null())
            {
                result->Error(kBadArgumentError, "Method invoked without args");
                return;
            }
            const JsonValue &args = *call.arguments();

            // TODO(awdavies): There's quite a wealth of arguments supplied with this
            // method, and they should be inspected/used.
            const JsonValue &client_id_json = args[0];
            const JsonValue &client_config = args[1];
            if (client_id_json.is_null())
            {
                result->Error(kBadArgumentError, "Could not set client, ID is null.");
                return;
            }
            if (client_config.is_null())
            {
                result->Error(kBadArgumentError,
                            "Could not set client, missing arguments.");
                return;
            }
            client_id_ = client_id_json.get<int>();
            input_action_ = "";
            auto input_action_json = client_config[kTextInputAction];
            if (input_action_json.is_string())
            {
                input_action_ = input_action_json.get<std::string>();
            }
            input_type_ = "";
            auto input_type_info_json = client_config[kTextInputType];
            if (input_type_info_json.is_object())
            {
                auto input_type_json =
                    input_type_info_json[kTextInputTypeName];
                if (input_type_json.is_string())
                {
                    input_type_ = input_type_json.get<std::string>();
                }
            }
            active_model_ = std::make_unique<TextInputModel>();

            SDL_StartTextInput();

            result->Success();
        })
    .def(kSetEditingStateMethod,
        [this](const JsonMethodCall &call, std::unique_ptr<JsonMethodResult> result)
        {
            if (!call.arguments() || call.arguments()->is_null()) {
                result->Error(kBadArgumentError, "Method invoked without args");
                return;
            }
            const JsonValue& args = *call.arguments();

            if (active_model_ == nullptr) {
                result->Error(
                    kInternalConsistencyError,
                    "Set editing state has been invoked, but no client is set.");
                return;
            }
            auto text = args[kTextKey];
            if (text.is_null()) {
                result->Error(kBadArgumentError,
                                "Set editing state has been invoked, but without text.");
                return;
            }
            auto selection_base = args[kSelectionBaseKey];
            auto selection_extent = args[kSelectionExtentKey];
            if (selection_base.is_null()) {
                result->Error(kInternalConsistencyError,
                                "Selection base/extent values invalid.");
                return;
            }
            // Flutter uses -1/-1 for invalid; translate that to 0/0 for the model.
            int base = selection_base.get<int>();
            int extent = selection_extent.get<int>();
            if (base == -1 && extent == -1) {
                base = extent = 0;
            }
            active_model_->SetText(text.get<std::string>());
            active_model_->SetSelection(TextRange(base, extent));

            result->Success();
        })
    ;
}

void TextInput::SendStateUpdate(const TextInputModel& model) {
  auto args = std::make_unique<JsonValue>(JsonValue::array());

  args->push_back(client_id_);

  TextRange selection = model.selection();
  JsonValue editing_state = JsonValue::object(
      {
          {kComposingBaseKey, -1},
          {kComposingExtentKey, -1},
          {kSelectionAffinityKey, kAffinityDownstream},
          {kSelectionBaseKey, selection.base()},
          {kSelectionExtentKey, selection.extent()},
          {kSelectionIsDirectionalKey, false},
          {kTextKey, model.GetText()},
      }
  );
  args->push_back(editing_state);

  channel_->InvokeMethod(kUpdateEditingStateMethod, std::move(args));
}

    bool TextInput::Dispatch(SDL_Event &e)
    {
        switch (e.type)
        {
        case SDL_EVENT_TEXT_INPUT:
            DispatchText(e.text);
            break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            DispatchKey(e.key);
            break;
        }
        }
        return true;
    }

void TextInput::DispatchText(const SDL_TextInputEvent &event) {
    active_model_->AddText(event.text);
    SendStateUpdate(*active_model_);
}

    void TextInput::DispatchKey(const SDL_KeyboardEvent &event)
    {
        if (active_model_ == nullptr)
        {
            return;
        }
        auto action = event.type;
        if (action == SDL_EVENT_KEY_DOWN || event.repeat != 0)
        {
            auto key = event.keysym.sym;
            switch (key)
            {
            case SDLK_LEFT:
                if (active_model_->MoveCursorBack())
                {
                    SendStateUpdate(*active_model_);
                }
                break;
            case SDLK_RIGHT:
                if (active_model_->MoveCursorForward())
                {
                    SendStateUpdate(*active_model_);
                }
                break;
            case SDLK_END:
                active_model_->MoveCursorToEnd();
                SendStateUpdate(*active_model_);
                break;
            case SDLK_HOME:
                active_model_->MoveCursorToBeginning();
                SendStateUpdate(*active_model_);
                break;
            case SDLK_BACKSPACE:
                if (active_model_->Backspace())
                {
                    SendStateUpdate(*active_model_);
                }
                break;
            case SDLK_DELETE:
                if (active_model_->Delete())
                {
                    SendStateUpdate(*active_model_);
                }
                break;
            case SDLK_RETURN:
                EnterPressed(active_model_.get());
                break;
            default:
                break;
            }
        }
    }

void TextInput::EnterPressed(TextInputModel* model) {
  if (input_type_ == kMultilineInputType) {
    model->AddCodePoint('\n');
    SendStateUpdate(*model);
  }
  //auto args = std::make_unique<rapidjson::Document>(rapidjson::kArrayType);
  auto args = std::make_unique<JsonValue>(JsonValue::array());
  args->push_back(client_id_);
  args->push_back(JsonValue(input_action_));

  channel_->InvokeMethod(kPerformActionMethod, std::move(args));
}

} // namespace attacus