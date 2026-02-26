/*
 * OpenGL loader.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cstring>
#include "Log.hpp"
#include "OpenGLLoader.hpp"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

#define LOAD_SYMBOL(type, name) dispatch->name = reinterpret_cast<type>(load(#name));

#define MAKE_VERSION_ID(major, minor) (((major) << 8) | (minor))

namespace Flux::OpenGLLoader {

Status detectLoadedVersion(OpenGLDispatch *dispatch) {
    const char *string;
    size_t length;
    GLuint major, minor;

    static const char *prefixes[] = {
        "OpenGL ES-CM ",
        "OpenGL ES-CL ",
        "OpenGL ES "
    };

    string = reinterpret_cast<const char *>(dispatch->glGetString(GL_VERSION));
    if (string == nullptr) {
        return Status::incompatibleDriver;
    }

    for (size_t p = 0; p < ARRAY_SIZE(prefixes); p++) {
        length = strlen(prefixes[p]);
        if (strncmp(string, prefixes[p], length) == 0) {
            string += length;
            break;
        }
    }

#if defined(_MSC_VER)
    sscanf_s(string, "%u.%u", &dispatch->loadedVersion.major, &dispatch->loadedVersion.minor);
#else
    sscanf(string, "%u.%u", &dispatch->loadedVersion.major, &dispatch->loadedVersion.minor);
#endif

    return Status::success;
}

Status loadSymbols(OpenGLDispatch *dispatch, GetSymbolAddress load) {
    Status status;
    GLuint versionId;

    LOAD_SYMBOL(PFNGLGETSTRINGPROC, glGetString);
    status = detectLoadedVersion(dispatch);
    if (status != Status::success) {
        return status;
    }

    FLUX_LOG_DEBUG("Loading symbols...");

    versionId = MAKE_VERSION_ID(dispatch->loadedVersion.major, dispatch->loadedVersion.minor);
    if (versionId >= MAKE_VERSION_ID(1, 0)) {
        ENUMERATE_OPENGL_SYMBOLS_10(LOAD_SYMBOL);
    }

    return Status::success;
}

}
