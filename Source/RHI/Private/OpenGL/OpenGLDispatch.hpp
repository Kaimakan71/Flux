/*
 * OpenGL dispatch.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include "OpenGL.h"

#define ENUMERATE_OPENGL_SYMBOLS_10(macro) \
    macro(PFNGLGETERRORPROC, glGetError)   \
    macro(PFNGLGETSTRINGPROC, glGetString) \

#define _DEFINE_MEMBER(type, name) type name;

namespace Flux {

typedef struct {
    unsigned int major;
    unsigned int minor;
} OpenGLVersion;

typedef struct {
    OpenGLVersion loadedVersion;

    ENUMERATE_OPENGL_SYMBOLS_10(_DEFINE_MEMBER);
} OpenGLDispatch;

}
