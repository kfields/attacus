#include <sokol_gfx.h>

#include <attacus/shell/gfx_view.h>

using namespace attacus;

/* a vertex buffer */
const float vertices[] = {
		// positions            // colors
		0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f};

sg_buffer_desc sgBufferDesc{
		.data = SG_RANGE(vertices)};

sg_shader_desc sgShaderDesc{
		.vs = {.source =
							 "#version 330\n"
							 "layout(location=0) in vec4 position;\n"
							 "layout(location=1) in vec4 color0;\n"
							 "out vec4 color;\n"
							 "void main() {\n"
							 "  gl_Position = position;\n"
							 "  color = color0;\n"
							 "}\n"},
		.fs = {.source =
							 "#version 330\n"
							 "in vec4 color;\n"
							 "out vec4 frag_color;\n"
							 "void main() {\n"
							 "  frag_color = color;\n"
							 "}\n"}};

/* default pass action (clear to grey) */
sg_pass_action pass_action = {0};

class ExampleCubesView : public GfxView
{
public:
	ExampleCubesView(View &parent, ViewParams params) : GfxView(parent, params)
	{
	}

	void Reset(ResetKind kind = ResetKind::kSoft) override
	{
		GfxView::Reset(kind);
		// bgfx::setViewClear(viewId(), BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x12345678, 1.0f, 0);
		// bgfx::setViewRect(viewId(), 0, 0, width(), height());
	}

	void Create() override
	{
		GfxView::Create();
		Reset();
		vbuf_ = sg_make_buffer(&sgBufferDesc);
		shd_ = sg_make_shader(&sgShaderDesc);
		//a pipeline state object (default render states are fine for triangle)
		sgPipelineDesc_ = {
				.shader = shd_,
				.layout{
						.attrs = {
								{.format = SG_VERTEXFORMAT_FLOAT3},
								{.format = SG_VERTEXFORMAT_FLOAT4}}}};

		pip_ = sg_make_pipeline(&sgPipelineDesc_);

		/* resource bindings */
		bindings_ = {
			.vertex_buffers { vbuf_ }
		};

	}

	void Draw() override
	{
		GfxView::Draw();

		int cur_width, cur_height;
		SDL_GL_GetDrawableSize(sdl_window_, &cur_width, &cur_height);
		sg_begin_default_pass(&pass_action, cur_width, cur_height);
		sg_apply_pipeline(pip_);
		sg_apply_bindings(&bindings_);
		sg_draw(0, 3, 1);
		sg_end_pass();
		sg_commit();
		SDL_GL_SwapWindow(sdl_window_);
	}

	// Data members
	sg_buffer vbuf_;
	sg_shader shd_;
	sg_pipeline_desc sgPipelineDesc_;
	sg_pipeline pip_;
	sg_bindings bindings_;
};

namespace attacus
{
	View *ProduceCubesView(View &parent, ViewParams params = ViewParams())
	{
		ExampleCubesView &view = *ExampleCubesView::Produce<ExampleCubesView>(parent, params);
		return &view;
	}
}