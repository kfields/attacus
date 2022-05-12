#pragma once

#ifdef SHELL_HEADER
#include SHELL_HEADER
#else
#include "window_base.h"
#endif

namespace attacus {

#ifdef SHELL_CLASS
using PlatformWindow = SHELL_CLASS;
#else
using PlatformWindow = WindowBase;
#endif

} //namespace attacus