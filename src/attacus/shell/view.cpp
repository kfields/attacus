#include <string>
#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include <SDL.h>
#include "SDL_syswm.h"

#include "SDL.h"
#include "SDL_syswm.h"

#include "view.h"

namespace attacus {

View::View(ViewParams params) : Surface(params),
    origin_(params.origin),
    name_(params.name) {
}

View::~View() {
}

void View::Create() {
    Surface::Create();
}

void View::Destroy() {
    Surface::Destroy();
}

} //namespace attacus