#include <iostream>

#include <glad/gl.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <bgfx/utils/utils.h>

#include <attacus/flutter/flutter_view.h>

#include "backing_surface_gfx.h"
#include "backing_layer_gfx.h"

#include "compositor_frame_gfx.h"
#include "compositor_gfx.h"

namespace attacus {

bx::AllocatorI* getDefaultAllocator()
{
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
    static bx::DefaultAllocator s_allocator;
    return &s_allocator;
BX_PRAGMA_DIAGNOSTIC_POP();
}

bx::AllocatorI* g_allocator = getDefaultAllocator();

CompositorGfx::CompositorGfx(FlutterView& view) : FlutterComponent(view) {
}

void CompositorGfx::Create() {
}

FlutterCompositor* CompositorGfx::InitCompositor() {
    FlutterCompositor& compositor = compositor_;
    compositor.struct_size = sizeof(FlutterCompositor);
    compositor.user_data = this;
    compositor.create_backing_store_callback =
        [](const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out, void* user_data) -> bool {
            CompositorGfx& self = *static_cast<CompositorGfx*>(user_data);
            bool result = self.DelegatedCreateBackingStore(config, backing_store_out);
            //SDL_GL_MakeCurrent(self.view().sdl_window_, self.view().context_);
            return result;
        };
    compositor.collect_backing_store_callback =
        [](const FlutterBackingStore* renderer, void* user_data) -> bool {
            return true; //TODO:LOL
            CompositorGfx &self = *static_cast<CompositorGfx*>(user_data);
            bool result = self.DelegatedCollectBackingStore(renderer);
            //SDL_GL_MakeCurrent(self.view().sdl_window_, self.view().context_);
            return result;
        };
    compositor.present_layers_callback =
        [](const FlutterLayer** layers, size_t layers_count, void* user_data) -> bool {
            CompositorGfx &self = *static_cast<CompositorGfx*>(user_data);
            bool result = self.DelegatedPresentLayers(layers, layers_count);
            //bool result = self.PresentLayers(layers, layers_count);
            //SDL_GL_MakeCurrent(self.view().sdl_window_, self.view().context_);
            return result;
        };
    compositor.avoid_backing_store_cache = false;

    return &compositor_;
}

bool CompositorGfx::DelegatedCreateBackingStore(const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out) {
    BackingSurfaceGfx* surface = GetCachedSurface();
    BackingSurfaceGfx** surface_p = &surface;
    if (surface) {
        return CreateBackingStore(*config, *backing_store_out, *surface);
    }

    std::unique_lock<std::mutex> lk(cv_m_);
    //std::cerr << "Waiting... \n";
    waiting_ = true;

    PushCallbackEvent(new Delegate([this, config, backing_store_out, surface_p]() -> void {
        surface_p[0] = AllocSurface(config->size);
        bgfx::frame(); //Force texture creation
        waiting_ = false;
        cv_.notify_all();
    }), this);

    cv_.wait(lk, [this]{return waiting_ == false;});
    //std::cerr << "...finished waiting";

    CreateBackingStore(*config, *backing_store_out, *surface);

    return true; //TODO: Need to get result from delegate function
}


BackingSurfaceGfx* CompositorGfx::AllocSurface(FlutterSize size) {
    auto width = size.width; auto height = size.height;

    SDL_GL_MakeCurrent(view().sdl_window_, view().gfx_context_);
    BackingSurfaceGfx* surface = BackingSurfaceGfx::Produce<BackingSurfaceGfx>(*this, SurfaceParams(Size(width, height)));
    return surface;
}

void CompositorGfx::FreeSurface(BackingSurfaceGfx& surface) {
    if (surface.used_)
        return;
    surface.used_ = true;
    free_surfaces_.push_back(&surface);
}

BackingSurfaceGfx* CompositorGfx::GetCachedSurface() {
    BackingSurfaceGfx* surface = nullptr;
    for (std::list<BackingSurfaceGfx*>::iterator it = free_surfaces_.begin(); it != free_surfaces_.end(); ++it) {
        surface = *it;
    }
    return surface;
}

bool CompositorGfx::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out, BackingSurfaceGfx& surface) {
    FlutterSize size = config.size;
    auto width = size.width; auto height = size.height;

    backing_store_out.user_data = &surface;
    backing_store_out.open_gl.type = kFlutterOpenGLTargetTypeTexture;
    FlutterOpenGLTexture& texOut = backing_store_out.open_gl.texture;
    texOut.target = GL_TEXTURE_2D;
    texOut.format = GL_RGBA8;
    texOut.name = surface.GetInternalTexture();
    //
    texOut.width = width;
    texOut.height = height;
    //
    texOut.destruction_callback = [](void* userdata){};
    texOut.user_data = &surface;
    return true;
}

bool CompositorGfx::DelegatedCollectBackingStore(const FlutterBackingStore* renderer) {
    std::unique_lock<std::mutex> lk(cv_m_);
    //std::cerr << "Waiting... \n";
    waiting_ = true;

    FlutterView::PushCallbackEvent(new Delegate([this, renderer]() -> void {
            CollectBackingStore(*renderer);
            waiting_ = false;
            cv_.notify_all();
        }), this);

    cv_.wait(lk, [this] {return waiting_ == false; });
    //std::cerr << "...finished waiting.";

    return true; //TODO: Need to get result from delegate function
}

bool CompositorGfx::CollectBackingStore(const FlutterBackingStore& renderer) {
    std::lock_guard<std::mutex> guard(guard_mutex_);
    Surface* surface = static_cast<Surface*>(renderer.user_data);
    surface->Destroy();
    return true;
}

bool CompositorGfx::DelegatedPresentLayers(const FlutterLayer** layers, size_t layers_count) {
    std::unique_lock<std::mutex> lk(cv_m_);
    //std::cerr << "Waiting... \n";
    waiting_ = true;

    FlutterView::PushCallbackEvent(new Delegate([this, layers, layers_count]() -> void {
            PresentLayers(layers, layers_count);
            waiting_ = false;
            cv_.notify_all();
        }), this);

    cv_.wait(lk, [this] {return waiting_ == false; });
    //std::cerr << "...finished waiting.";

    return true; //TODO: Need to get result from delegate function
}

bool CompositorGfx::PresentLayers(const FlutterLayer** layers, size_t layers_count) {
    //std::lock_guard<std::mutex> guard(render_mutex_);

    CompositorFrameGfx* frame = new CompositorFrameGfx(*this);

    for (int i = 0; i < layers_count; ++i) {
        const FlutterLayer& _layer = *layers[i];
        if (_layer.type == kFlutterLayerContentTypeBackingStore) {
            BackingSurfaceGfx& surface = *static_cast<BackingSurfaceGfx*>(_layer.backing_store->user_data);
            BackingLayerGfx* layer = new BackingLayerGfx(*this, surface, _layer.offset, _layer.size);
            layer->Create();
            frame->AddLayer(layer);
        }
    }

    frames_.push(frame);

    /*rendering_ = true;
    std::unique_lock<std::mutex> lk(render_cv_m_);
    render_cv_.wait(lk, [this]{return rendering_ == false;});*/

    //std::this_thread::sleep_for(throttle_);

    return true;
}

void CompositorGfx::Draw() {
    //std::lock_guard<std::mutex> guard(render_mutex_);

    //rendering_ = true;
    //render_cv_.notify_all();

    if (!frame_ && frames_.empty())
        return;

    if (frames_.empty()) {
        frame_->Draw();
    }

    while (!frames_.empty()) {
        auto front = frames_.front();
        if (frame_)
            frame_->Destroy();
        frame_ = front;
        frames_.pop();
        frame_->Draw();
    }

    /*bool hasFrames = !frames_.empty();
    while (!frames_.empty()) {
        auto frame = frames_.front();
        frames_.pop();

        frame->Draw();

        frame->Destroy();
    }

    if (hasFrames) {
        rendering_ = false;
        render_cv_.notify_all();
    }*/

    /*rendering_ = false;
    render_cv_.notify_all();*/
}


} // namespace attacus