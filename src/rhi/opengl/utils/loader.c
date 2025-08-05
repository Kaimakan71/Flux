/*
 * OpenGL symbol loader.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "internal/rhi/glUtils.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

typedef struct {
    void **destination;
    const char *name;
} Symbol;

PFNGLCLEARCOLORPROC glClearColor = NULL;
PFNGLGETSTRINGPROC glGetString = NULL;

static const Symbol symbols10[] = {
    { (void**)&glClearColor, "glClearColor" },
};

static void
loadSymbolGroup(size_t symbolCount, const Symbol *symbols, GlUtilsGetSymbolAddress getSymbolAddress)
{
    for (size_t s = 0; s < symbolCount; s++) {
        *symbols[s].destination = getSymbolAddress(symbols[s].name);
    }
}

static GLboolean
getVersion(GlUtilsLibraryVersion *version)
{
    const char *string;
    size_t length;

    static const char *prefixes[] = {
        "OpenGL ES-CM ",
        "OpenGL ES-CL ",
        "OpenGL ES "
    };

    string = (const char*)glGetString(GL_VERSION);
    if (string == NULL) {
        return GL_FALSE;
    }

    for (size_t p = 0; p < ARRAY_LENGTH(prefixes); p++) {
        length = strlen(prefixes[p]);
        if (strncmp(string, prefixes[p], length) == 0) {
            string += length;
            break;
        }
    }

#if defined(_MSC_VER)
    sscanf_s(string, "%d.%d", &version->major, &version->minor);
#else
    sscanf(string, "%d.%d", &version->major, &version->minor);
#endif

    return GL_TRUE;
}

GLboolean
glUtilsLoadSymbols(GlUtilsGetSymbolAddress getSymbolAddress, GlUtilsLibraryVersion *version)
{
    /* Required by getVersion() */
    glGetString = (PFNGLGETSTRINGPROC)getSymbolAddress("glGetString");
    if (glGetString == NULL) {
        return GL_FALSE;
    }

    if (!getVersion(version)) {
        return GL_FALSE;
    }

    if (version->major >= 1) {
        if (version->minor >= 0) {
            loadSymbolGroup(ARRAY_LENGTH(symbols10), symbols10, getSymbolAddress);
        }
    }

    return GL_TRUE;
}
