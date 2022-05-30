#include <iostream>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <bgfx/utils/utils.h>

#include "flutter_composite_view.h"
#include "backing_store.h"

struct PosTexcoordVertex {
    float x;
    float y;
    float z;
    float u;
    float v;
    float w;

    static void Init() {
        ms_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .end();
    }

    static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout PosTexcoordVertex::ms_layout;

static PosTexcoordVertex s_quadVertices[4] = {
    {-1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f },
    {-1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f },
    { 1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f }
};

static const uint16_t s_quadIndices[6] = {
    0, 1, 2,
    1, 3, 2
};

namespace attacus
{

FlutterCompositeView::FlutterCompositeView(View& parent, ViewParams params) : FlutterView(parent, params)
{
    view_id_ = 0;
}

void FlutterCompositeView::Create() {
    FlutterView::Create();

    bgfx::setViewClear(viewId()
        , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
        //, 0x303030ff
        , 0x12345678
        , 1.0f
        , 0
    );

    bgfx::setViewRect(viewId(), 0, 0, width(), height());

    PosTexcoordVertex::Init();

    uniform_ = bgfx::createUniform("uniform_",  bgfx::UniformType::Sampler);
    program_ = loadProgram("vs_update", "fs_update_cmp");

    vbh_ = bgfx::createVertexBuffer(
        bgfx::makeRef(s_quadVertices, sizeof(s_quadVertices)),
        PosTexcoordVertex::ms_layout);
    ibh_ = bgfx::createIndexBuffer(
        bgfx::makeRef(s_quadIndices, sizeof(s_quadIndices)));
}

void FlutterCompositeView::InitProjectArgs(FlutterProjectArgs& args) {
    FlutterView::InitProjectArgs(args);
    InitCompositor(compositor_);
    args.compositor = &compositor_;
}

void FlutterCompositeView::InitCompositor(FlutterCompositor& compositor) {
    compositor.struct_size = sizeof(FlutterCompositor);
    compositor.user_data = this;
    compositor.create_backing_store_callback =
        [](const FlutterBackingStoreConfig* config, FlutterBackingStore* backing_store_out, void* user_data) -> bool {
            FlutterCompositeView &self = *static_cast<FlutterCompositeView*>(user_data);
            return self.CreateBackingStore(*config, *backing_store_out);
        };
    compositor.collect_backing_store_callback =
        [](const FlutterBackingStore* renderer, void* user_data) -> bool {
            FlutterCompositeView &self = *static_cast<FlutterCompositeView*>(user_data);
            return self.CollectBackingStore(*renderer);
        };
    compositor.present_layers_callback =
        [](const FlutterLayer** layers, size_t layers_count, void* user_data) -> bool {
            FlutterCompositeView &self = *static_cast<FlutterCompositeView*>(user_data);
            return self.PresentLayers(layers, layers_count);
        };
}
/*
bool FlutterCompositeView::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out) {
    //SDL_GL_MakeCurrent(sdl_window_, gfx_context_);
    FlutterSize size = config.size;
    auto width = size.width; auto height = size.height;
    Surface& surface = *Surface::Produce(SurfaceParams(Size(width, height)));
    backing_store_out.user_data = &surface;
    surface.CreateTexture();
    //Already set
    backing_store_out.open_gl.type = kFlutterOpenGLTargetTypeTexture;
    FlutterOpenGLTexture* texOut = &backing_store_out.open_gl.texture;
    //Todo: this is duplicated in TextureRegistrar.  DRY!
    texOut->target = GL_TEXTURE_2D;
    texOut->name = surface.GetInternalTexture();
    texOut->width = width;
    texOut->height = height;
    texOut->format = GL_RGBA8;
    //texOut->format = GL_BGRA;
    //texOut->format = GL_BGRA8_EXT;
    //
    //texOut->destruction_callback = [](void* userdata){};
    texOut->user_data = &surface;
    return true;
}
*/

bool FlutterCompositeView::CreateBackingStore(const FlutterBackingStoreConfig& config, FlutterBackingStore& backing_store_out) {
    FlutterSize size = config.size;
    auto width = size.width; auto height = size.height;
    BackingStore& surface = *BackingStore::Produce<BackingStore>(SurfaceParams(Size(width, height)));
    backing_store_out.user_data = &surface;
    backing_store_out.open_gl.type = kFlutterOpenGLTargetTypeFramebuffer;
    FlutterOpenGLFramebuffer& fbOut = backing_store_out.open_gl.framebuffer;
    //fbOut.target = GL_TEXTURE_2D;
    //fbOut.target = GL_RENDERBUFFER;
    //fbOut.target = 0;
    fbOut.target = GL_RGBA8;
    fbOut.name = surface.framebuffer_id;
    //
    fbOut.destruction_callback = [](void* userdata){};
    fbOut.user_data = &surface;
    return true;
}

bool FlutterCompositeView::CollectBackingStore(const FlutterBackingStore& renderer) {
    Surface& surface = *static_cast<Surface*>(renderer.user_data);
    surface.Destroy();
    return true;
}

bool FlutterCompositeView::PresentLayers(const FlutterLayer** layers, size_t layers_count) {
    layers_ = layers;
    layers_count_ = layers_count;

    std::unique_lock<std::mutex> lk(cv_m_);
    std::cerr << "Waiting... \n";
    waiting_ = true;
    cv_.wait(lk, [this]{return waiting_ == false;});
    std::cerr << "...finished waiting. i == 1\n";

    return true;
}

void FlutterCompositeView::Draw() {
    if (!waiting_)
        return;
    /*if (layers_ == nullptr) {
        return;
    }*/
    for (int i = 0; i < layers_count_; ++i) {
        const FlutterLayer& layer = *layers_[i];
        if (layer.type == kFlutterLayerContentTypeBackingStore) {
            DrawBackingStore(layer);
        }
    }
    //layers_ = nullptr;
    //layers_count_ = 0;
    waiting_ = false;
    cv_.notify_all();
}

void FlutterCompositeView::DrawBackingStore(const FlutterLayer& layer) {
    BackingStore& surface = *static_cast<BackingStore*>(layer.backing_store->user_data);
    int width = surface.width();
    int height = surface.height();
    bgfx::overrideInternal(surface.texture_, surface.texture_id);


    float scaleMat[16];
    float rotateMat[16];
    float transMat[16];
    float scale = 1.0f;

    bx::mtxScale(scaleMat, width * scale, height * scale, 1.0f);
    //bx::mtxRotateXYZ(rotateMat, 0.0f, 0.0f, glm::radians(angle_));
    //bx::mtxTranslate(transMat, x_, y_, 0.0f);

    //float tmpMat[16];
    //float tmpMat2[16];

    //bx::mtxMul(tmpMat, scaleMat, rotateMat);
    //bx::mtxMul(tmpMat2, tmpMat, transMat);

    //bgfx::setTransform(tmpMat2);
    bgfx::setTransform(scaleMat);

    bgfx::setVertexBuffer(viewId(), vbh_);
    bgfx::setIndexBuffer(ibh_);
    bgfx::setTexture(viewId(), uniform_, surface.texture_);

    //uint64_t state = BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA;
    //bgfx::setState(state);

    bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A);
    //bgfx::setState(BGFX_STATE_DEFAULT);

    bgfx::submit(viewId(), program_);


}

} // namespace attacus