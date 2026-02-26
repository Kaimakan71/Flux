/*
 * OpenGL loader.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "OpenGLDispatch.hpp"
#include "Status.hpp"

namespace Flux::OpenGLLoader {

typedef void (*Symbol)(void);
typedef Symbol (*GetSymbolAddress)(const char *name);

Status detectLoadedVersion(OpenGLDispatch *dispatch);

Status loadSymbols(OpenGLDispatch *dispatch, GetSymbolAddress load);

}
