#include <iostream>

#include <glad/gl.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <bgfx/utils/utils.h>

#include <vg/vg.h>

#include <attacus/flutter/flutter_view.h>

#include "backing_surface.h"
#include "compositor.h"

namespace attacus
{

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

class CompositorLayer : public Component {
public:
    CompositorLayer(vg::Context& vg) : vg_(&vg) {}
    CompositorLayer(vg::Context& vg, FlutterPoint origin, FlutterSize size) :
    vg_(&vg), origin_(origin), size_(size) {
        std::cerr << "Layer: " << "x: " << origin.x << "y: " << origin.y << std::endl;
    }
    virtual void Draw(){}
    // Data members
    vg::Context* vg_;
    FlutterPoint origin_;
    FlutterSize size_;
};

class CompositorFrame : public CompositorLayer {
public:
    CompositorFrame(vg::Context& vg) : CompositorLayer(vg) {}
    virtual void Draw() {
        for (std::vector<CompositorLayer*>::iterator it = layers_.begin(); it != layers_.end(); ++it){
            CompositorLayer* layer = *it;
            layer->Draw();
        }
    }
    void Destroy() {
        for (std::vector<CompositorLayer*>::iterator it = layers_.begin(); it != layers_.end(); ++it) {
            CompositorLayer* layer = *it;
            layer->Destroy();
        }
    }
    void AddLayer(CompositorLayer* layer) { layers_.push_back(layer); }
    // Data members
    std::vector<CompositorLayer*> layers_;
};

class BackingLayer : public CompositorLayer {
public:
    BackingLayer(vg::Context& vg, BackingSurface& surface, FlutterPoint origin, FlutterSize size) : CompositorLayer(vg, origin, size),
    surface_(&surface)
    {}
    void Create() {
        //bgfx::overrideInternal(surface_->texture_, surface_->texture_id);
        width_ = surface_->width();
        height_ = surface_->height();
        //ImageHandle createImage(Context* ctx, uint32_t flags, const bgfx::TextureHandle& bgfxTextureHandle);
        image_ = createImage(vg_, vg::ImageFlags::Filter_NearestUV, surface_->texture_);
        //ImagePatternHandle createImagePattern(Context* ctx, float cx, float cy, float w, float h, float angle, ImageHandle image);
        imgPaint_ = createImagePattern(vg_, 0, 0, width_, height_, 0.0f, image_);
    }

    void Destroy() override {
        vg::destroyImage(vg_, image_);
        Component::Destroy();
    }

    void Draw() override {
        vg::beginPath(vg_);
        //void rect(Context* ctx, float x, float y, float w, float h);
        vg::rect(vg_, 0, 0, width_, height_);
        //void fillPath(Context* ctx, ImagePatternHandle img, Color color, uint32_t flags);
        //vg::fillPath(vg, imgPaint_, vg::color4f(1.0f, 1.0f, 1.0f, 1.0f), vg::FillFlags::ConvexAA);
        vg::fillPath(vg_, imgPaint_, vg::color4f(1.0f, 1.0f, 1.0f, 1.0f), vg::FillFlags::Concave);
    }
    // Data members
    int width_;
    int height_;
    vg::ImageHandle image_;
    vg::ImagePatternHandle imgPaint_;
    BackingSurface* surface_ = nullptr;
};

Compositor::Compositor(FlutterView& view) : FlutterComponent(view) {
}

void Compositor::Create() {
    const vg::ContextConfig cfg = {
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
    vg_ = vg::createContext(g_allocator, &cfg);
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
            return self.DelegatedCreateBackingStore(config, backing_store_out);
            //return self.DelegatedCreateBackingStore(config, backing_store_out);
        };
    compositor.collect_backing_store_callback =
        [](const FlutterBackingStore* renderer, void* user_data) -> bool {
            Compositor &self = *static_cast<Compositor*>(user_data);
            //return self.CollectBackingStore(*renderer);
            return self.DelegatedCollectBackingStore(renderer);
        };
    compositor.present_layers_callback =
        [](const FlutterLayer** layers, size_t layers_count, void* user_data) -> bool {
            Compositor &self = *static_cast<Compositor*>(user_data);
            return self.DelegatedPresentLayers(layers, layers_count);
            //return self.PresentLayers(layers, layers_count);
        };
    return &compositor_;
}

bool Compositor::DelegatedCreateBackingStore(const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out) {
    std::unique_lock<std::mutex> lk(cv_m_);
    std::cerr << "Waiting... \n";
    waiting_ = true;

    FlutterView::PushCallbackEvent(new Delegate([this, config, backing_store_out]() -> void {
        CreateBackingStore(*config, *backing_store_out);
        waiting_ = false;
        cv_.notify_all();
    }), this);

    cv_.wait(lk, [this]{return waiting_ == false;});
    std::cerr << "...finished waiting. i == 1\n";

    return true; //TODO: Need to get result from delegate function
}

bool Compositor::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out) {
    SDL_GL_MakeCurrent(view().sdl_window_, view().gfx_context_);
    FlutterSize size = config.size;
    auto width = size.width; auto height = size.height;
    BackingSurface& surface = *BackingSurface::Produce<BackingSurface>(SurfaceParams(Size(width, height)));
    backing_store_out.user_data = &surface;
    //backing_store_out.open_gl.type = kFlutterOpenGLTargetTypeFramebuffer;
    //FlutterOpenGLFramebuffer& fbOut = backing_store_out.open_gl.framebuffer;
    FlutterOpenGLTexture& fbOut = backing_store_out.open_gl.texture;
    fbOut.target = GL_TEXTURE_2D;
    //fbOut.target = GL_RENDERBUFFER;
    //fbOut.target = 0;
    //fbOut.target = GL_RGBA8;
    //fbOut.name = surface.framebuffer_id;
    fbOut.format = GL_RGBA8;
    fbOut.name = surface.texture_id;
    //
    fbOut.destruction_callback = [](void* userdata){};
    fbOut.user_data = &surface;
    return true;
}

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
    return true;
    std::lock_guard<std::mutex> guard(guard_mutex_);
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

bool Compositor::PresentLayers(const FlutterLayer** layers, size_t layers_count) {
    std::lock_guard<std::mutex> guard(guard_mutex_);

    CompositorFrame& frame = *new CompositorFrame(*vg_);

    for (int i = 0; i < layers_count; ++i) {
        const FlutterLayer& _layer = *layers[i];
        if (_layer.type == kFlutterLayerContentTypeBackingStore) {
            BackingSurface& surface = *static_cast<BackingSurface*>(_layer.backing_store->user_data);
            BackingLayer* layer = new BackingLayer(*vg_, surface, _layer.offset, _layer.size);
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

    if (!frames_.empty()) {
        auto front = frames_.front();
        if (frame_)
            frame_->Destroy();
        frame_ = front;
        frames_.pop();
    }

    vg::begin(vg_, view().viewId(), view().width(), view().height(), 1.0f);
    frame_->Draw();
    vg::end(vg_);

    //TODO:Needs to get called at end of frame not here. 
    vg::frame(vg_);

    /*auto front = frames_.front();
    frame_ = front;
    frames_.pop();

    vg::begin(vg_, view().viewId(), view().width(), view().height(), 1.0f);
    frame_->Draw();
    vg::end(vg_);
    //TODO:Needs to get called at end of frame not here. 
    vg::frame(vg_);*/

    /*while (!frames_.empty()) {
        auto front = frames_.front();
        if (frame_)
            frame_->Destroy();
        frame_ = front;
        frames_.pop();
        vg::begin(vg_, view().viewId(), view().width(), view().height(), 1.0f);
        frame_->Draw();
        vg::end(vg_);
    }

    vg::begin(vg_, view().viewId(), view().width(), view().height(), 1.0f);
    frame_->Draw();
    vg::end(vg_);
    //TODO:Needs to get called at end of frame not here. 
    vg::frame(vg_);*/
}


} // namespace attacus