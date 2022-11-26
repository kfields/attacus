#pragma once

#include <nohlmann/json.hpp>
using json = nlohmann::json;

//#include <glad/gl.h>

#include <attacus/shell/gfx_view.h>
#include "flutter_embedder.h"

struct SDL_Window;
struct SDL_Cursor;

namespace attacus {

class Window;

class FlutterMessenger;
class FlutterRunner;

class Compositor;

class IsolateComponent;
class PlatformComponent;
class NavigationComponent;
class CursorComponent;
class MouseInput;
class TextInput;
class TextureRegistrar;
class ViewRegistry;

class FlutterView : public GfxView {
public:
    FlutterView(View& parent, ViewParams params = ViewParams());

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


    bool Dispatch(SDL_Event& event) override;
    // Events
    void OnResize(SDL_Event& event) override;
    void OnSize() override;
    void OnShow() override;
    //
    void UpdateSize(size_t width, size_t height, float pixelRatio, bool maximized);
    //Accessors
    FlutterEngine& engine() { return engine_; }
    FlutterMessenger& messenger() { return *messenger_; }
    FlutterRunner& runner() { return *runner_; }

    Compositor& compositor() { return *compositor_; }

    IsolateComponent& isolate() { return *isolate_; }
    PlatformComponent& platform() { return *platform_; }
    NavigationComponent& navigation() { return *navigation_; }
    CursorComponent& cursor() { return *cursor_; }
    MouseInput& mouseInput() { return *mouseInput_; }
    TextInput& textInput() { return *textInput_; }
    TextureRegistrar& textureRegistrar() { return *textureRegistrar_; }
    ViewRegistry& viewRegistry() { return *viewRegistry_; }
    //Data members
    void* context_ = nullptr;
    void* resource_context_ = nullptr;
    //GLADloadfunc gl_proc_resolver = nullptr;
    //
    float scaleFactor_ = 1.0f;
    //
    FlutterEngine engine_ = nullptr;
    FlutterEngineProcTable engine_api_{0};
    FlutterMessenger* messenger_ = nullptr;
    FlutterRunner* runner_ = nullptr;
    //
    Compositor* compositor_ = nullptr;
    //
    IsolateComponent* isolate_ = nullptr;
    PlatformComponent* platform_ = nullptr;
    NavigationComponent* navigation_ = nullptr;
    CursorComponent* cursor_ = nullptr;
    MouseInput* mouseInput_ = nullptr;
    TextInput* textInput_ = nullptr;

    TextureRegistrar* textureRegistrar_ = nullptr;
    ViewRegistry* viewRegistry_ = nullptr;
};

} //namespace attacus