#include <string>
#include <iostream>
#include <format>

#include <glad/gl.h>
#include "SDL.h"
#include "SDL_syswm.h"

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define SOKOL_EXTERNAL_GL_LOADER
#include <sokol_gfx.h>

#include "gfx_view.h"
#include "gfx.h"

namespace attacus
{

  Gfx *Gfx::instance_ = nullptr;

  Gfx::Gfx(GfxView &view) : view_(&view),
                            resetFlags_(0),
                            debugFlags_(0)
  {
    instance_ = this;
  }

  void Gfx::Create()
  {
    gl_proc_resolver_ = (GLADloadfunc)SDL_GL_GetProcAddress;
    int version = gladLoadGL(gl_proc_resolver_);
    std::cout << std::format("OpenGL {}.{} loaded\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    /* setup sokol_gfx */
    sg_desc sgDesc{0};
    sg_setup(&sgDesc);
  }

  void Gfx::Reset()
  {
    // bgfx::reset(view().width(), view().height(), resetFlags_);
  }
} // namespace attacus