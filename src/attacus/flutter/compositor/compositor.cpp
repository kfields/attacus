#include <iostream>

#include <glad/gl.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <bgfx/utils/utils.h>

#include <vg/vg.h>

#include <attacus/flutter/flutter_view.h>

#include "backing_surface.h"
#include "backing_layer.h"

#include "compositor_frame.h"
#include "compositor.h"

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

Compositor::Compositor(FlutterView& view) : FlutterComponent(view) {
}

void Compositor::Create() {
    /*const vg::ContextConfig cfg = {
        256,                             // m_MaxGradients
        256,                             // m_MaxImagePatterns
        8,                               // m_MaxFonts
        32,                              // m_MaxStateStackSize
        64,                              // m_MaxImages
        256,                             // m_MaxCommandLists
        65536,                           // m_MaxVBVertices
        vg::ImageFlags::Filter_Bilinear, // m_FontAtlasImageFlags
        16                               // m_MaxCommandListDepth
    };
    vg_ = vg::createContext(g_allocator, &cfg);*/
    vg_ = vg::createContext(g_allocator);
    if (!vg_) {
        bx::debugPrintf("Failed to create vg-renderer context.\n");
        return;
    }
}

FlutterCompositor* Compositor::InitCompositor() {
    FlutterCompositor& compositor = compositor_;
    compositor.struct_size = sizeof(FlutterCompositor);
    compositor.user_data = this;
    compositor.create_backing_store_callback =
        [](const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out, void* user_data) -> bool {
            Compositor& self = *static_cast<Compositor*>(user_data);
            bool result = self.DelegatedCreateBackingStore(config, backing_store_out);
            SDL_GL_MakeCurrent(self.view().sdl_window_, self.view().context_);
            return result;
        };
    compositor.collect_backing_store_callback =
        [](const FlutterBackingStore* renderer, void* user_data) -> bool {
            Compositor &self = *static_cast<Compositor*>(user_data);
            bool result = self.DelegatedCollectBackingStore(renderer);
            SDL_GL_MakeCurrent(self.view().sdl_window_, self.view().context_);
            return result;
        };
    compositor.present_layers_callback =
        [](const FlutterLayer** layers, size_t layers_count, void* user_data) -> bool {
            Compositor &self = *static_cast<Compositor*>(user_data);
            //bool result = self.DelegatedPresentLayers(layers, layers_count);
            bool result = self.PresentLayers(layers, layers_count);
            SDL_GL_MakeCurrent(self.view().sdl_window_, self.view().context_);
            return result;
        };
    return &compositor_;
}

/*bool Compositor::DelegatedCreateBackingStore(const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out) {
    std::unique_lock<std::mutex> lk(cv_m_);
    std::cerr << "Waiting... \n";
    waiting_ = true;

    PushCallbackEvent(new Delegate([this, config, backing_store_out]() -> void {
        BackingSurface* surface = GetCachedSurface();
        if (surface) {
            CreateBackingStore(*config, *backing_store_out, *surface);
            waiting_ = false;
            cv_.notify_all();
            return;
        }
        surface = AllocSurface(config->size);
        PushCallbackEvent(new Delegate([this, config, backing_store_out, surface]() -> void {
            CreateBackingStore(*config, *backing_store_out, *surface);
            waiting_ = false;
            cv_.notify_all();
        }), this);
    }), this);

    cv_.wait(lk, [this]{return waiting_ == false;});
    std::cerr << "...finished waiting. i == 1\n";

    return true; //TODO: Need to get result from delegate function
}*/

bool Compositor::DelegatedCreateBackingStore(const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out) {
    BackingSurface* surface = GetCachedSurface();
    if (surface) {
        return CreateBackingStore(*config, *backing_store_out, *surface);
    }

    std::unique_lock<std::mutex> lk(cv_m_);
    std::cerr << "Waiting... \n";
    waiting_ = true;

    PushCallbackEvent(new Delegate([this, config, backing_store_out]() -> void {
        BackingSurface* surface = AllocSurface(config->size);
        PushCallbackEvent(new Delegate([this, config, backing_store_out, surface]() -> void {
            CreateBackingStore(*config, *backing_store_out, *surface);
            waiting_ = false;
            cv_.notify_all();
        }), this);
    }), this);

    cv_.wait(lk, [this]{return waiting_ == false;});
    std::cerr << "...finished waiting. i == 1\n";

    return true; //TODO: Need to get result from delegate function
}


BackingSurface* Compositor::AllocSurface(FlutterSize size) {
    auto width = size.width; auto height = size.height;

    SDL_GL_MakeCurrent(view().sdl_window_, view().gfx_context_);
    BackingSurface* surface = BackingSurface::Produce<BackingSurface>(*this, SurfaceParams(Size(width, height)));
    return surface;
}

void Compositor::FreeSurface(BackingSurface& surface) {
    surface.used_ = true;
    free_surfaces_.push_back(&surface);
}

BackingSurface* Compositor::GetCachedSurface() {
    for (std::list<BackingSurface*>::iterator it = free_surfaces_.begin(); it != free_surfaces_.end(); ++it) {
        BackingSurface* surface = *it;
        return surface;
    }
    return nullptr;
}

bool Compositor::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out, BackingSurface& surface) {
    std::lock_guard<std::mutex> guard(guard_mutex_);

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

/*bool Compositor::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out, BackingSurface& surface) {
    bgfx::overrideInternal(surface.texture_, surface.texture_id);
    backing_store_out.user_data = &surface;
    backing_store_out.open_gl.type = kFlutterOpenGLTargetTypeFramebuffer;
    FlutterOpenGLFramebuffer& fbOut = backing_store_out.open_gl.framebuffer;
    fbOut.target = GL_RGBA8;
    fbOut.name = surface.framebuffer_id;
    //
    fbOut.destruction_callback = [](void* userdata){};
    fbOut.user_data = &surface;
    return true;
}
*/

bool Compositor::DelegatedCollectBackingStore(const FlutterBackingStore* renderer) {
    std::unique_lock<std::mutex> lk(cv_m_);
    std::cerr << "Waiting... \n";
    waiting_ = true;

    FlutterView::PushCallbackEvent(new Delegate([this, renderer]() -> void {
            CollectBackingStore(*renderer);
            waiting_ = false;
            cv_.notify_all();
        }), this);

    cv_.wait(lk, [this] {return waiting_ == false; });
    std::cerr << "...finished waiting. i == 1\n";

    return true; //TODO: Need to get result from delegate function
}

bool Compositor::CollectBackingStore(const FlutterBackingStore& renderer) {
    std::lock_guard<std::mutex> guard(guard_mutex_);
    return true;
    Surface* surface = static_cast<Surface*>(renderer.user_data);
    surface->Destroy();
    return true;
}

bool Compositor::DelegatedPresentLayers(const FlutterLayer** layers, size_t layers_count) {
    std::unique_lock<std::mutex> lk(cv_m_);
    std::cerr << "Waiting... \n";
    waiting_ = true;

    FlutterView::PushCallbackEvent(new Delegate([this, layers, layers_count]() -> void {
        PresentLayers(layers, layers_count);
        waiting_ = false;
        cv_.notify_all();
    }), this);

    cv_.wait(lk, [this]{return waiting_ == false;});
    std::cerr << "...finished waiting. i == 1\n";

    return true; //TODO: Need to get result from delegate function
}

/*bool Compositor::PresentLayers(const FlutterLayer** layers, size_t layers_count) {
    std::lock_guard<std::mutex> guard(guard_mutex_);
    SDL_GL_MakeCurrent(view().sdl_window_, view().gfx_context_);

    CompositorFrame& frame = *new CompositorFrame(*this);

    for (int i = 0; i < layers_count; ++i) {
        const FlutterLayer& _layer = *layers[i];
        if (_layer.type == kFlutterLayerContentTypeBackingStore) {
            BackingSurface& surface = *static_cast<BackingSurface*>(_layer.backing_store->user_data);
            BackingLayer* layer = new BackingLayer(*this, surface, _layer.offset, _layer.size);
            layer->Create();
            frame.AddLayer(layer);
        }
    }

    frames_.push(&frame);

    return true;
}*/

bool Compositor::PresentLayers(const FlutterLayer** layers, size_t layers_count) {
    std::lock_guard<std::mutex> guard(guard_mutex_);

    //SDL_GL_MakeCurrent(view().sdl_window_, view().gfx_context_);

    CompositorFrame& frame = *new CompositorFrame(*this);

    for (int i = 0; i < layers_count; ++i) {
        const FlutterLayer& _layer = *layers[i];
        if (_layer.type == kFlutterLayerContentTypeBackingStore) {
            BackingSurface& surface = *static_cast<BackingSurface*>(_layer.backing_store->user_data);
            BackingLayer* layer = new BackingLayer(*this, surface, _layer.offset, _layer.size);
            layer->Create();
            frame.AddLayer(layer);
        }
    }

    frames_.push(&frame);

    return true;
}

void Compositor::Draw() {
    std::lock_guard<std::mutex> guard(guard_mutex_);

    if (!frame_ && frames_.empty())
        return;

    if (frames_.empty()) {
        vg::begin(vg_, view().viewId(), view().width(), view().height(), 1.0f);
        frame_->Draw();
        vg::end(vg_);
        //TODO:Needs to get called at end of frame not here. 
        vg::frame(vg_);
    }
    while (!frames_.empty()) {
        auto front = frames_.front();
        if (frame_)
            frame_->Destroy();
        frame_ = front;
        frames_.pop();
        vg::begin(vg_, view().viewId(), view().width(), view().height(), 1.0f);
        frame_->Draw();
        vg::end(vg_);
    }

    vg::frame(vg_);
}


} // namespace attacus