#include <string>
#include <iostream>
#include <format>

#include <glad/gl.h>
#include "SDL.h"
#include "SDL_syswm.h"

#include "gfx_view.h"
#include "gfx.h"

namespace attacus
{

  void *GfxView::current_context_ = nullptr;
  void *GfxView::gfx_context_ = nullptr;
  static bool initialized_ = false;

  GfxView::GfxView(View &parent, ViewParams params) : View(parent, params),
                                                      time_offset_(0)
  {
  }

  void GfxView::Create()
  {
    View::Create();
    CreateGfx();
  }

  void GfxView::PreRender()
  {
    View::PreRender();
    SDL_GL_MakeCurrent(sdl_window_, gfx_context_);
  }

  void GfxView::PostRender()
  {
    Touch();
    View::PostRender();
  }

  void *GfxView::CreateContext()
  {
    void *context = nullptr;
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    if (current_context_)
    {
      SDL_GL_MakeCurrent(sdl_window_, current_context_);
      context = SDL_GL_CreateContext(sdl_window_);
    }
    else
    {
      context = SDL_GL_CreateContext(sdl_window_);
    }
    current_context_ = context;
    if (context == NULL)
    {
      std::cout << std::format("Can't create opengl context: {}\n", SDL_GetError());
      return nullptr;
    }

    return context;
  }

  void GfxView::CreateGfx()
  {
    if (!gfx_context_)
    {
        gfx_context_ = CreateContext();
        if (gfx_context_ == NULL)
        {
          std::cout << std::format("Can't create opengl context for bgfx: {}\n", SDL_GetError());
          return;
        }
        InitGfx();
    }
    if (SDL_GL_SetSwapInterval(1) < 0)
    {
      std::cout << std::format("Couldn't enable vsync: {}\n", SDL_GetError());
    }
  }

  void GfxView::CreateFramebuffer()
  {
    View::CreateFramebuffer();
  }

  void GfxView::Reset(ResetKind kind)
  {
    CreateFramebuffer();
  }

  void GfxView::OnSize()
  {
    Reset(ResetKind::kHard);
  }

  void GfxView::InitGfx()
  {
    if (initialized_)
    {
      gfx_ = Gfx::instance_;
      return;
    }

    gfx_ = new Gfx(*this);
    gfx_->Create();
    initialized_ = true;
  }

} // namespace attacus