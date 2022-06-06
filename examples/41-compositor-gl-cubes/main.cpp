#include <bgfx/bgfx.h>
#include <bgfx/bgfx_p.h>
#include <bx/math.h>
#include <bx/timer.h>

#include <bgfx/utils/utils.h>

#include <attacus/flutter/flutter_composite_view_gl.h>
#include <attacus/flutter/flutter_messenger.h>
#include <attacus/flutter/standard_method_channel.h>
#include <attacus/flutter/components/texture_registrar.h>
#include <attacus/shell/gfx_view.h>

#include <examples/example_app.h>

#include "logo.h"

using namespace attacus;

static constexpr char kChannelName[] = "attacus/texture";
static constexpr char kCreateMethod[] = "create";
static constexpr char kDisposeMethod[] = "dispose";
static constexpr char kWidthKey[] = "width";
static constexpr char kHeightKey[] = "height";

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

static const uint16_t s_cubeTriStrip[] =
{
	0, 1, 2,
	3,
	7,
	1,
	5,
	0,
	4,
	2,
	6,
	7,
	4,
	5,
};

static const uint16_t s_cubeLineList[] =
{
	0, 1,
	0, 2,
	0, 4,
	1, 3,
	1, 5,
	2, 3,
	2, 6,
	3, 7,
	4, 5,
	4, 6,
	5, 7,
	6, 7,
};

static const uint16_t s_cubeLineStrip[] =
{
	0, 2, 3, 1, 5, 7, 6, 4,
	0, 2, 6, 4, 5, 7, 3, 1,
	0,
};

static const uint16_t s_cubePoints[] =
{
	0, 1, 2, 3, 4, 5, 6, 7
};

static const char* s_ptNames[]
{
	"Triangle List",
	"Triangle Strip",
	"Lines",
	"Line Strip",
	"Points",
};

static const uint64_t s_ptState[]
{
	UINT64_C(0),
	BGFX_STATE_PT_TRISTRIP,
	BGFX_STATE_PT_LINES,
	BGFX_STATE_PT_LINESTRIP,
	BGFX_STATE_PT_POINTS,
};
BX_STATIC_ASSERT(BX_COUNTOF(s_ptState) == BX_COUNTOF(s_ptNames) );

class ExampleCubesView : public GfxView {
public:
	ExampleCubesView(View& parent, ViewParams params) : GfxView(parent, params),
		pt_(0),
		r_(true),
		g_(true),
		b_(true),
		a_(true)
	{
    }

    virtual void Create() override {
        GfxView::Create();

        CreateFramebuffer();
        
        bgfx::setViewClear(viewId()
            , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
            //, 0x303030ff
            , 0x12345678
            , 1.0f
            , 0
        );

        bgfx::setViewRect(viewId(), 0, 0, width(), height());

        // Create vertex stream declaration.
        PosColorVertex::init();

        // Create static vertex buffer.
        vbh_ = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
            , PosColorVertex::ms_layout
        );

        // Create static index buffer for triangle list rendering.
        ibh_[0] = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
        );

        // Create static index buffer for triangle strip rendering.
        ibh_[1] = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeTriStrip, sizeof(s_cubeTriStrip))
        );

        // Create static index buffer for line list rendering.
        ibh_[2] = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeLineList, sizeof(s_cubeLineList))
        );

        // Create static index buffer for line strip rendering.
        ibh_[3] = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeLineStrip, sizeof(s_cubeLineStrip))
        );

        // Create static index buffer for point list rendering.
        ibh_[4] = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubePoints, sizeof(s_cubePoints))
        );

        // Create program from shaders.
        program_ = loadProgram("vs_cubes", "fs_cubes");

        time_offset_ = bx::getHPCounter();

    }

    virtual void Draw() override {
        GfxView::Draw();

		float time = (float)((bx::getHPCounter() - time_offset_) / double(bx::getHPFrequency()));

		const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

		// Set view and projection matrix for view 0.
		{
			float view[16];
			bx::mtxLookAt(view, eye, at);

			float proj[16];
			bx::mtxProj(proj, 60.0f, float(width()) / float(height()), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(viewId(), view, proj);

			// Set view 0 default viewport.
			bgfx::setViewRect(viewId(), 0, 0, uint16_t(width()), uint16_t(height()));
		}

		bgfx::IndexBufferHandle ibh = ibh_[pt_];
		uint64_t state = 0
			| (r_ ? BGFX_STATE_WRITE_R : 0)
			| (g_ ? BGFX_STATE_WRITE_G : 0)
			| (b_ ? BGFX_STATE_WRITE_B : 0)
			| (a_ ? BGFX_STATE_WRITE_A : 0)
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			| s_ptState[pt_]
			;

		// Submit 11x11 cubes.
		for (uint32_t yy = 0; yy < 11; ++yy)
		{
			for (uint32_t xx = 0; xx < 11; ++xx)
			{
				float mtx[16];
				bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
				mtx[12] = -15.0f + float(xx) * 3.0f;
				mtx[13] = -15.0f + float(yy) * 3.0f;
				mtx[14] = 0.0f;

				// Set model matrix for rendering.
				bgfx::setTransform(mtx);

				// Set vertex and index buffer.
				bgfx::setVertexBuffer(0, vbh_);
				bgfx::setIndexBuffer(ibh);

				// Set render states.
				bgfx::setState(state);

				// Submit primitive for rendering to view 0.
				bgfx::submit(viewId(), program_);
			}
		}
    }

    // Data members
	bgfx::VertexBufferHandle vbh_;
	bgfx::IndexBufferHandle ibh_[BX_COUNTOF(s_ptState)];
	bgfx::ProgramHandle program_;
	int64_t time_offset_;
	int32_t pt_;

	bool r_;
	bool g_;
	bool b_;
	bool a_;
};

class ExampleCubes : public FlutterCompositeViewGL {
public:
    ExampleCubes(View& parent, ViewParams params = ViewParams()) : FlutterCompositeViewGL(parent, params) {}
    
    virtual void Create() override {
        FlutterCompositeViewGL::Create();
        
        channel_ = new StandardMethodChannel(messenger(), kChannelName);

        new StandardMethod(*channel_, kCreateMethod,
            [this](const MethodCall<>& call, std::unique_ptr<MethodResult<>> result) {
                //auto args = call.arguments();
                const auto& args = std::get<EncodableMap>(*call.arguments());
                //auto width = std::get<double>(args[0]);
                //auto width = std::get<double>(args.at(EncodableValue("width")));
                auto width_iter = args.find(EncodableValue(std::string(kWidthKey)));
                if (width_iter == args.end()) {
                result->Error("Argument error",
                                "Missing argument while trying to activate system cursor");
                return;
                }
                const uint16_t width = std::get<double>(width_iter->second);

                //auto height = std::get<double>(args[1]);
                //auto height = std::get<double>(args.at(EncodableValue("height")));
                auto height_iter = args.find(EncodableValue(std::string(kHeightKey)));
                if (height_iter == args.end()) {
                result->Error("Argument error",
                                "Missing argument while trying to activate system cursor");
                return;
                }
                const uint16_t height = std::get<double>(height_iter->second);

                cubes_view_ = ExampleCubesView::Produce<ExampleCubesView>(*this, ViewParams(Size(width, height)));
                auto texId = textureRegistrar().RegisterTexture(*cubes_view_);

                result->Success(texId);
            });

        new StandardMethod(*channel_, kDisposeMethod,
            [this](const MethodCall<>& call, std::unique_ptr<MethodResult<>> result) {
                auto args = call.arguments();
                auto id = std::get<int>(args[0]);
                result->Success();
            });

    }

    // Data members
    ExampleCubesView* cubes_view_ = nullptr;
    MethodChannel<>* channel_ = nullptr;
};

int main(int argc, char** argv) {
	ExampleApp& app = *ExampleApp::Produce(ExampleParams(
        "41-compositor-gl-cubes",
        "Show cubes as texture using compositor.",
        "https://kfields.github.io/attacus/examples.html#cubes"
    ));
	FlutterView& flutter = *ExampleCubes::Produce<ExampleCubes>(app);
	return app.Run();
}
