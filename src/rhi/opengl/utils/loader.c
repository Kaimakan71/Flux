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

PFNGLBLENDFUNCPROC glBlendFunc = NULL;
PFNGLCLEARPROC glClear = NULL;
PFNGLCLEARCOLORPROC glClearColor = NULL;
PFNGLCLEARDEPTHPROC glClearDepth = NULL;
PFNGLCLEARSTENCILPROC glClearStencil = NULL;
PFNGLCOLORMASKPROC glColorMask = NULL;
PFNGLCULLFACEPROC glCullFace = NULL;
PFNGLDEPTHFUNCPROC glDepthFunc = NULL;
PFNGLDEPTHMASKPROC glDepthMask = NULL;
PFNGLDEPTHRANGEPROC glDepthRange = NULL;
PFNGLDISABLEPROC glDisable = NULL;
PFNGLDRAWBUFFERPROC glDrawBuffer = NULL;
PFNGLENABLEPROC glEnable = NULL;
PFNGLFINISHPROC glFinish = NULL;
PFNGLFLUSHPROC glFlush = NULL;
PFNGLFRONTFACEPROC glFrontFace = NULL;
PFNGLGETBOOLEANVPROC glGetBooleanv = NULL;
PFNGLGETDOUBLEVPROC glGetDoublev = NULL;
PFNGLGETERRORPROC glGetError = NULL;
PFNGLGETFLOATVPROC glGetFloatv = NULL;
PFNGLGETINTEGERVPROC glGetIntegerv = NULL;
PFNGLGETSTRINGPROC glGetString = NULL;
PFNGLGETTEXIMAGEPROC glGetTexImage = NULL;
PFNGLGETTEXLEVELPARAMETERFVPROC glGetTexLevelParameterfv = NULL;
PFNGLGETTEXLEVELPARAMETERIVPROC glGetTexLevelParameteriv = NULL;
PFNGLGETTEXPARAMETERFVPROC glGetTexParameterfv = NULL;
PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv = NULL;
PFNGLHINTPROC glHint = NULL;
PFNGLISENABLEDPROC glIsEnabled = NULL;
PFNGLLINEWIDTHPROC glLineWidth = NULL;
PFNGLLOGICOPPROC glLogicOp = NULL;
PFNGLPIXELSTOREFPROC glPixelStoref = NULL;
PFNGLPIXELSTOREIPROC glPixelStorei = NULL;
PFNGLPOINTSIZEPROC glPointSize = NULL;
PFNGLPOLYGONMODEPROC glPolygonMode = NULL;
PFNGLREADBUFFERPROC glReadBuffer = NULL;
PFNGLREADPIXELSPROC glReadPixels = NULL;
PFNGLSCISSORPROC glScissor = NULL;
PFNGLSTENCILFUNCPROC glStencilFunc = NULL;
PFNGLSTENCILMASKPROC glStencilMask = NULL;
PFNGLSTENCILOPPROC glStencilOp = NULL;
PFNGLTEXIMAGE1DPROC glTexImage1D = NULL;
PFNGLTEXIMAGE2DPROC glTexImage2D = NULL;
PFNGLTEXPARAMETERFPROC glTexParameterf = NULL;
PFNGLTEXPARAMETERFVPROC glTexParameterfv = NULL;
PFNGLTEXPARAMETERIPROC glTexParameteri = NULL;
PFNGLTEXPARAMETERIVPROC glTexParameteriv = NULL;
PFNGLVIEWPORTPROC glViewport = NULL;

PFNGLBINDTEXTUREPROC glBindTexture = NULL;
PFNGLCOPYTEXIMAGE1DPROC glCopyTexImage1D = NULL;
PFNGLCOPYTEXIMAGE2DPROC glCopyTexImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE1DPROC glCopyTexSubImage1D = NULL;
PFNGLCOPYTEXSUBIMAGE2DPROC glCopyTexSubImage2D = NULL;
PFNGLDELETETEXTURESPROC glDeleteTextures = NULL;
PFNGLDRAWARRAYSPROC glDrawArrays = NULL;
PFNGLDRAWELEMENTSPROC glDrawElements = NULL;
PFNGLGENTEXTURESPROC glGenTextures = NULL;
PFNGLISTEXTUREPROC glIsTexture = NULL;
PFNGLPOLYGONOFFSETPROC glPolygonOffset = NULL;
PFNGLTEXSUBIMAGE1DPROC glTexSubImage1D = NULL;
PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D = NULL;

PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D = NULL;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = NULL;
PFNGLTEXIMAGE3DPROC glTexImage3D = NULL;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = NULL;

PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage = NULL;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = NULL;

PFNGLBLENDCOLORPROC glBlendColor = NULL;
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = NULL;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays = NULL;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements = NULL;
PFNGLPOINTPARAMETERFPROC glPointParameterf = NULL;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv = NULL;
PFNGLPOINTPARAMETERIPROC glPointParameteri = NULL;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv = NULL;

PFNGLGENQUERIESPROC glGenQueries = NULL;

static const Symbol symbols10[] = {
    { (void**)&glBlendFunc, "glBlendFunc" },
    { (void**)&glClear, "glClear" },
    { (void**)&glClearColor, "glClearColor" },
    { (void**)&glClearDepth, "glClearDepth" },
    { (void**)&glClearStencil, "glClearStencil" },
    { (void**)&glColorMask, "glColorMask" },
    { (void**)&glCullFace, "glCullFace"},
    { (void**)&glDepthFunc, "glDepthFunc" },
    { (void**)&glDepthMask, "glDepthMask" },
    { (void**)&glDepthRange, "glDepthRange" },
    { (void**)&glDisable, "glDisable" },
    { (void**)&glDrawBuffer, "glDrawBuffer" },
    { (void**)&glEnable, "glEnable" },
    { (void**)&glFinish, "glFinish" },
    { (void**)&glFlush, "glFlush" },
    { (void**)&glFrontFace, "glFrontFace" },
    { (void**)&glGetBooleanv, "glGetBooleanv" },
    { (void**)&glGetDoublev, "glGetDoublev" },
    { (void**)&glGetError, "glGetError" },
    { (void**)&glGetFloatv, "glGetFloatv" },
    { (void**)&glGetIntegerv, "glGetIntegerv" },
    { (void**)&glGetTexImage, "glGetTexImage" },
    { (void**)&glGetTexLevelParameterfv, "glGetTexLevelParameterfv" },
    { (void**)&glGetTexLevelParameteriv, "glGetTexLevelParameteriv" },
    { (void**)&glGetTexParameterfv, "glGetTexParameterfv" },
    { (void**)&glGetTexParameteriv, "glGetTexParameteriv" },
    { (void**)&glHint, "glHint" },
    { (void**)&glIsEnabled, "glIsEnabled" },
    { (void**)&glLineWidth, "glLineWidth" },
    { (void**)&glLogicOp, "glLogicOp" },
    { (void**)&glPixelStoref, "glPixelStoref" },
    { (void**)&glPixelStorei, "glPixelStorei" },
    { (void**)&glPointSize, "glPointSize" },
    { (void**)&glPolygonMode, "glPolygonMode" },
    { (void**)&glReadBuffer, "glReadBuffer" },
    { (void**)&glReadPixels, "glReadPixels" },
    { (void**)&glScissor, "glScissor" },
    { (void**)&glStencilFunc, "glStencilFunc" },
    { (void**)&glStencilMask, "glStencilMask" },
    { (void**)&glStencilOp, "glStencilOp" },
    { (void**)&glTexImage1D, "glTexImage1D" },
    { (void**)&glTexImage2D, "glTexImage2D" },
    { (void**)&glTexParameterf, "glTexParameterf" },
    { (void**)&glTexParameterfv, "glTexParameterfv" },
    { (void**)&glTexParameteri, "glTexParameteri" },
    { (void**)&glTexParameteriv, "glTexParameteriv" },
    { (void**)&glViewport, "glViewport" },
};

static const Symbol symbols11[] = {
    { (void**)&glBindTexture, "glBindTexture" },
    { (void**)&glCopyTexImage1D, "glCopyTexImage1D" },
    { (void**)&glCopyTexImage2D, "glCopyTexImage2D" },
    { (void**)&glCopyTexSubImage1D, "glCopyTexSubImage1D" },
    { (void**)&glCopyTexSubImage2D, "glCopyTexSubImage2D" },
    { (void**)&glDeleteTextures, "glDeleteTextures" },
    { (void**)&glDrawArrays, "glDrawArrays" },
    { (void**)&glDrawElements, "glDrawElements" },
    { (void**)&glGenTextures, "glGenTextures" },
    { (void**)&glIsTexture, "glIsTexture" },
    { (void**)&glPolygonOffset, "glPolygonOffset" },
    { (void**)&glTexSubImage1D, "glTexSubImage1D" },
    { (void**)&glTexSubImage2D, "glTexSubImage2D" },
};

static const Symbol symbols12[] = {
    { (void**)&glCopyTexSubImage3D, "glCopyTexSubImage3D" },
    { (void**)&glDrawRangeElements, "glDrawRangeElements" },
    { (void**)&glTexImage3D, "glTexImage3D" },
    { (void**)&glTexSubImage3D, "glTexSubImage3D" },
};

static const Symbol symbols13[] = {
    { (void**)&glActiveTexture, "glActiveTexture" },
    { (void**)&glCompressedTexImage1D, "glCompressedTexImage1D" },
    { (void**)&glCompressedTexImage2D, "glCompressedTexImage2D" },
    { (void**)&glCompressedTexImage3D, "glCompressedTexImage3D" },
    { (void**)&glCompressedTexSubImage1D, "glCompressedTexSubImage1D" },
    { (void**)&glCompressedTexSubImage2D, "glCompressedTexSubImage2D" },
    { (void**)&glCompressedTexSubImage3D, "glCompressedTexSubImage3D" },
    { (void**)&glGetCompressedTexImage, "glGetCompressedTexImage" },
    { (void**)&glSampleCoverage, "glSampleCoverage" },
};

static const Symbol symbols14[] = {
    { (void**)&glBlendColor, "glBlendColor" },
    { (void**)&glBlendEquation, "glBlendEquation" },
    { (void**)&glBlendFuncSeparate, "glBlendFuncSeparate" },
    { (void**)&glMultiDrawArrays, "glMultiDrawArrays" },
    { (void**)&glMultiDrawElements, "glMultiDrawElements" },
    { (void**)&glPointParameterf, "glPointParameterf" },
    { (void**)&glPointParameterfv, "glPointParameterfv" },
    { (void**)&glPointParameteri, "glPointParameteri" },
    { (void**)&glPointParameteriv, "glPointParameteriv" },
};

static const Symbol symbols15[] = {
    { (void**)&glGenQueries, "glGenQueries" },
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

        if (version->minor >= 1) {
            loadSymbolGroup(ARRAY_LENGTH(symbols11), symbols11, getSymbolAddress);
        }

        if (version->minor >= 2) {
            loadSymbolGroup(ARRAY_LENGTH(symbols12), symbols12, getSymbolAddress);
        }

        if (version->minor >= 3) {
            loadSymbolGroup(ARRAY_LENGTH(symbols13), symbols13, getSymbolAddress);
        }

        if (version->minor >= 4) {
            loadSymbolGroup(ARRAY_LENGTH(symbols14), symbols14, getSymbolAddress);
        }

        if (version->minor >= 5) {
            loadSymbolGroup(ARRAY_LENGTH(symbols15), symbols15, getSymbolAddress);
        }
    }

    return GL_TRUE;
}
