#pragma once

#include <nohlmann/json.hpp>
using json = nlohmann::json;

#include <attacus/shell/view.h>
#include "flutter_embedder.h"

struct SDL_Window;
struct SDL_Cursor;
typedef union SDL_Event SDL_Event;

namespace attacus {

class Window;

class FlutterMessenger;
class FlutterRunner;

class IsolateComponent;
class PlatformComponent;
class NavigationComponent;
class CursorComponent;
class MouseInput;
class TextInput;
class TextureRegistrar;

class FlutterView : public View {
public:
    FlutterView(WindowParams params = WindowParams());
    virtual ~FlutterView();
    virtual void Create() override;
    virtual void Destroy() override;
    static FlutterView& Produce(WindowParams params = WindowParams()) {
        FlutterView& r = *new FlutterView(params);
        r.Create();
        return r;
    }
    void Render();
    virtual bool Dispatch(SDL_Event& event) override;
    // Events
    virtual void OnResize(SDL_Event& event) override;
    virtual void OnSize() override;
    virtual void OnShow() override;
    //
    void UpdateSize(size_t width, size_t height, float pixelRatio, bool maximized);
    //Accessors
    FlutterEngine& engine() { return engine_; }
    FlutterMessenger& messenger() { return *messenger_; }
    FlutterRunner& runner() { return *runner_; }

    IsolateComponent& isolate() { return *isolate_; }
    PlatformComponent& platform() { return *platform_; }
    NavigationComponent& navigation() { return *navigation_; }
    CursorComponent& cursor() { return *cursor_; }
    MouseInput& mouseInput() { return *mouseInput_; }
    TextInput& textInput() { return *textInput_; }
    TextureRegistrar& textureRegistrar() { return *textureRegistrar_; }
    //Data members
    float scaleFactor_ = 1.0f;
    //
    FlutterEngine engine_ = nullptr;
    FlutterMessenger* messenger_ = nullptr;
    FlutterRunner* runner_ = nullptr;
    //
    IsolateComponent* isolate_ = nullptr;
    PlatformComponent* platform_ = nullptr;
    NavigationComponent* navigation_ = nullptr;
    CursorComponent* cursor_ = nullptr;
    MouseInput* mouseInput_ = nullptr;
    TextInput* textInput_ = nullptr;

    TextureRegistrar* textureRegistrar_ = nullptr;
};

} //namespace attacus