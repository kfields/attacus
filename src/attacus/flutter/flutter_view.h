#pragma once

#include <nohlmann/json.hpp>
using json = nlohmann::json;

#include <glad/gl.h>

#include <attacus/shell/gfx_view.h>
#include "flutter_embedder.h"

struct SDL_Window;
struct SDL_Cursor;

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

class FlutterView : public GfxView {
public:
    FlutterView(View& parent, ViewParams params = ViewParams());
    //FlutterView(const FlutterView&);
    template<typename T = FlutterView>
    static T* Produce(View& parent, ViewParams params = ViewParams()) {
        T* c = new T(parent, params);
        c->Create();
        return c;
    }

    void Create() override;
    void CreateGfx() override;
    virtual void InitRendererConfig(FlutterRendererConfig& config);
    virtual void InitProjectArgs(FlutterProjectArgs& args);
    virtual void InitEngine(FlutterRendererConfig& config, FlutterProjectArgs& args);

    void Destroy() override;
    virtual ~FlutterView();


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
    void* context_ = nullptr;
    void* resource_context_ = nullptr;
    GLADloadfunc gl_proc_resolver = nullptr;
    //
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