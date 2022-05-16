#include <cassert>
#include <chrono>
#include <iostream>

#include <filesystem>
namespace fs = std::filesystem;

#include <SDL.h>

#include <attacus/app.h>

#include "flutter_window.h"
#include "flutter_embedder.h"
#include "flutter_messenger.h"
#include "flutter_runner.h"

#include "components/isolate.h"
#include "components/platform.h"
#include "components/navigation.h"
#include "components/cursor.h"
#include "components/mouse_input.h"
#include "components/text_input.h"

#include "components/texture_registrar.h"

#define FLUTTER_ENGINE_VERSION 1

namespace attacus
{

FlutterWindow::FlutterWindow(WindowParams params) : GfxWindow(params)
{
    messenger_ = new FlutterMessenger(*this);
    runner_ = new FlutterRunner(*this);

    isolate_ = new IsolateComponent(*this);
    platform_ = new PlatformComponent(*this);
    navigation_ = new NavigationComponent(*this);
    cursor_ = new CursorComponent(*this);
    mouseInput_ = new MouseInput(*this);
    textInput_ = new TextInput(*this);

    textureRegistrar_ = new TextureRegistrar(*this);
}

FlutterWindow::~FlutterWindow()
{
}

void FlutterWindow::Create()
{
    GfxWindow::Create();

    FlutterRendererConfig config = {};
    config.type = kOpenGL;
    config.open_gl.struct_size = sizeof(config.open_gl);
    config.open_gl.make_current = [](void *userdata) -> bool
    {
        auto self = *static_cast<FlutterWindow*>(userdata);
        auto window = self.sdl_window_;
        SDL_GL_MakeCurrent(window, self.context_);
        return true;
    };
    config.open_gl.make_resource_current = [](void *userdata) -> bool
    {
        auto self = *static_cast<FlutterWindow*>(userdata);
        auto window = self.sdl_window_;
        SDL_GL_MakeCurrent(window, self.resource_context_);
        return true;
    };
    config.open_gl.clear_current = [](void *userdata) -> bool
    {
        auto self = *static_cast<FlutterWindow*>(userdata);
        auto window = self.sdl_window_;
        SDL_GL_MakeCurrent(window, nullptr);
        return true;
    };
    config.open_gl.present = [](void *userdata) -> bool
    {
        auto self = *static_cast<FlutterWindow*>(userdata);
        auto window = self.sdl_window_;
        SDL_GL_SwapWindow(window);
        return true;
    };
    config.open_gl.fbo_callback = [](void *userdata) -> uint32_t
    {
        return 0; // FBO0
    };

    config.open_gl.gl_proc_resolver = [](void *userdata, const char *name) -> void *
    {
        return SDL_GL_GetProcAddress(name);
    };

    config.open_gl.gl_external_texture_frame_callback = 
        [](void *userdata, int64_t texId, size_t width, size_t height, FlutterOpenGLTexture*  texOut) -> bool
    {
        auto self = *static_cast<FlutterWindow*>(userdata);
        return self.textureRegistrar().CopyTexture(texId, width, height, texOut);
    };

    // This directory is generated by `flutter build bundle`.
    fs::path project_path = fs::current_path();
    // fs::path project_path(SDL_GetBasePath());
    fs::path assets_path = project_path / "build" / "flutter_assets";
    fs::path icudtl_path = project_path / "build" / "icudtl.dat";

    FlutterProjectArgs args = {};
    args.struct_size = sizeof(FlutterProjectArgs);
    args.assets_path = _strdup(assets_path.string().c_str());
    args.icu_data_path = _strdup(icudtl_path.string().c_str());
    args.platform_message_callback = [](const FlutterPlatformMessage *message, void *user_data)
    {
        FlutterWindow &self = *static_cast<FlutterWindow*>(user_data);
        self.messenger().Receive(*message);
    };
    args.custom_task_runners = &runner_->custom_task_runners;

    //SDL_GL_MakeCurrent(sdl_window_, nullptr);

    //FlutterEngineResult result = FlutterEngineRun(FLUTTER_ENGINE_VERSION, &config, &args, this, &engine_);
    FlutterEngineResult result = FlutterEngineInitialize(FLUTTER_ENGINE_VERSION, &config, &args, this, &engine_);
    if (result != kSuccess || engine_ == nullptr)
    {
        std::cout << "Could not initialize the Flutter Engine." << std::endl;
        return;
    }

    messenger().Create();
    runner().Create();

    isolate().Create();
    platform().Create();
    navigation().Create();
    cursor().Create();
    mouseInput().Create();
    textInput().Create();

    textureRegistrar().Create();

    result = FlutterEngineRunInitialized(engine_);
    if (result != kSuccess || engine_ == nullptr)
    {
        std::cout << "Could not run the Flutter Engine." << std::endl;
        return;
    }
}

void FlutterWindow::Destroy() {
    FlutterEngineResult result = FlutterEngineDeinitialize(engine_);
    GfxWindow::Destroy();
}

void FlutterWindow::Render()
{
}

void FlutterWindow::OnResize(SDL_Event &event)
{
    GfxWindow::OnResize(event);
    UpdateSize(event.window.data1, event.window.data2, 1.0, false);
}

void FlutterWindow::OnSize()
{
    GfxWindow::OnSize();
    UpdateSize(width(), height(), 1.0, false);
}

void FlutterWindow::OnShow()
{
    FlutterWindowMetricsEvent event = {};
    event.struct_size = sizeof(event);
    event.width = width();
    event.height = height();
    event.pixel_ratio = 1.0;
    FlutterEngineSendWindowMetricsEvent(engine_, &event);
}

void FlutterWindow::UpdateSize(size_t width, size_t height, float pixelRatio, bool maximized)
{
    //  Round up the physical window size to a multiple of the pixel ratio
    width = std::ceil(width / pixelRatio) * pixelRatio;
    height = std::ceil(height / pixelRatio) * pixelRatio;

    FlutterWindowMetricsEvent event = {0};
    event.struct_size = sizeof(event);
    event.width = width * scaleFactor_;
    event.height = height * scaleFactor_;
    event.pixel_ratio = pixelRatio * scaleFactor_;

    FlutterEngineSendWindowMetricsEvent(engine_, &event);
}

bool FlutterWindow::Dispatch(SDL_Event &e)
{
    /*
    switch (e.type)
    {
    }
    */

    mouseInput_->Dispatch(e);
    textInput_->Dispatch(e);

    return GfxWindow::Dispatch(e);
}

} // namespace attacus