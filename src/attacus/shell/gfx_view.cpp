#include <string>
#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include "SDL.h"
#include "SDL_syswm.h"

#include <bx/timer.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "gfx_view.h"
#include "gfx.h"

namespace attacus
{

  void *GfxView::current_context_ = nullptr;
  void *GfxView::gfx_context_ = nullptr;
  static bool initialized_ = false;

  GfxView::GfxView(View &parent, ViewParams params) : View(parent, params),
                                                      time_offset_(bx::getHPCounter())
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
    //bgfx::touch(viewId());
  }

  void GfxView::PostRender()
  {
    // bgfx::frame(capture_);
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
      std::cout << fmt::format("Can't create opengl context: {}\n", SDL_GetError());
      return nullptr;
    }

    return context;
  }

  /*void GfxView::CreateGfx() {
      if (!gfx_context_) {
          gfx_context_ = CreateContext();
          if (gfx_context_ == NULL) {
              std::cout << fmt::format("Can't create opengl context for bgfx: {}\n", SDL_GetError());
              return;
          }
      }
      InitGfx();
  }*/

  void GfxView::CreateGfx()
  {
    if (!gfx_context_)
    {
      if (viewId() == 0)
      {
        InitGfx();
        auto internal = bgfx::getInternalData();
        current_context_ = gfx_context_ = internal->context;
        SDL_GL_MakeCurrent(sdl_window_, current_context_);
      }
      else
      {
        gfx_context_ = CreateContext();
        if (gfx_context_ == NULL)
        {
          std::cout << fmt::format("Can't create opengl context for bgfx: {}\n", SDL_GetError());
          return;
        }
        InitGfx();
      }
    }
    if (SDL_GL_SetSwapInterval(1) < 0)
    {
      std::cout << fmt::format("Couldn't enable vsync: {}\n", SDL_GetError());
    }
  }

  void GfxView::CreateFramebuffer()
  {
    View::CreateFramebuffer();
    bgfx::setViewFrameBuffer(viewId(), frameBuffer_);
  }

  void GfxView::Reset(ResetKind kind)
  {
    if (kind == ResetKind::kSoft)
    {
      // if (bgfx::isValid(frameBuffer_))
      bgfx::setViewFrameBuffer(viewId(), frameBuffer_);
    }
    else
    {
      if (bgfx::isValid(frameBuffer_))
        destroy(frameBuffer_);

      CreateFramebuffer();
    }
  }

  void GfxView::OnSize()
  {
    // Reset(ResetKind::kHard);
    if (viewId() == 0)
    {
      Reset();
    }
    else
    {
      Reset(ResetKind::kHard);
      // Reset();
    }
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