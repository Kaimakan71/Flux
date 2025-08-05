/*
 * OpenGL utilities.
 * Copyright (c) 2025, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifndef _RHI_GL_UTILS_H
#define _RHI_GL_UTILS_H

#define GL_FALSE 0
#define GL_TRUE  1

#define GL_POINTS         0x0000
#define GL_LINES          0x0001
#define GL_LINE_LOOP      0x0002
#define GL_LINE_STRIP     0x0003
#define GL_TRIANGLES      0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN   0x0006
#define GL_QUADS          0x0007

#define GL_NEVER    0x0200
#define GL_LESS     0x0201
#define GL_EQUAL    0x0202
#define GL_LEQUAL   0x0203
#define GL_GREATER  0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL   0x0206
#define GL_ALWAYS   0x0207

#define GL_SRC_COLOR           0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA           0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA           0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR           0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE  0x0308

#define GL_FRONT_LEFT     0x0400
#define GL_FRONT_RIGHT    0x0401
#define GL_BACK_LEFT      0x0402
#define GL_BACK_RIGHT     0x0403
#define GL_FRONT          0x0404
#define GL_BACK           0x0405
#define GL_LEFT           0x0406
#define GL_RIGHT          0x0407
#define GL_FRONT_AND_BACK 0x0408

#define GL_NO_ERROR 0
#define GL_INVALID_ENUM                  0x0500
#define GL_INVALID_VALUE                 0x0501
#define GL_INVALID_OPERATION             0x0502
#define GL_STACK_OVERFLOW                0x0503
#define GL_STACK_UNDERFLOW               0x0504
#define GL_OUT_OF_MEMORY                 0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_CONTEXT_LOST                  0x0507

#define GL_CW  0x0900
#define GL_CCW 0x0901

#define GL_POINT_SIZE             0x0B11
#define GL_POINT_SIZE_RANGE       0x0B12
#define GL_POINT_SIZE_GRANULARITY 0x0B13

#define GL_LINE_SMOOTH            0x0B20
#define GL_LINE_WIDTH             0x0B21
#define GL_LINE_WIDTH_RANGE       0x0B22
#define GL_LINE_WIDTH_GRANULARITY 0x0B23

#define GL_POLYGON_MODE   0x0B40
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_CULL_FACE      0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE     0x0B46

#define GL_DEPTH_RANGE       0x0B70
#define GL_DEPTH_TEST        0x0B71
#define GL_DEPTH_WRITEMASK   0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC        0x0B74

#define GL_STENCIL_TEST            0x0B90
#define GL_STENCIL_CLEAR_VALUE     0x0B91
#define GL_STENCIL_FUNC            0x0B92
#define GL_STENCIL_VALUE_MASK      0x0B93
#define GL_STENCIL_FAIL            0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF             0x0B97
#define GL_STENCIL_WRITEMASK       0x0B98

#define GL_VIEWPORT 0x0BA2

#define GL_DITHER 0x0BD0

#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND     0x0BE2

#define GL_LOGIC_OP_MODE 0x0BF0

#define GL_DRAW_BUFFER 0x0C01
#define GL_READ_BUFFER 0x0C02

#define GL_SCISSOR_BOX  0x0C10
#define GL_SCISSOR_TEST 0x0C11


#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK   0x0C23

#define GL_DOUBLEBUFFER 0x0C32
#define GL_STEREO       0x0C33

#define GL_LINE_SMOOTH_HINT    0x0C52
#define GL_POLYGON_SMOOTH_HINT 0x0C53

#define GL_UNPACK_SWAP_BYTES  0x0CF0
#define GL_UNPACK_LSB_FIRST   0x0CF1
#define GL_UNPACK_ROW_LENGTH  0x0CF2
#define GL_UNPACK_SKIP_ROWS   0x0CF3
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_ALIGNMENT   0x0CF5

#define GL_PACK_SWAP_BYTES  0x0D00
#define GL_PACK_LSB_FIRST   0x0D01
#define GL_PACK_ROW_LENGTH  0x0D02
#define GL_PACK_SKIP_ROWS   0x0D03
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_ALIGNMENT   0x0D05

#define GL_MAX_TEXTURE_SIZE  0x0D33
#define GL_MAX_VIEWPORT_DIMS 0x0D3A

#define GL_SUBPIXEL_BITS 0x0D50

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1

#define GL_TEXTURE_WIDTH        0x1000
#define GL_TEXTURE_HEIGHT       0x1001
#define GL_TEXTURE_BORDER_COLOR 0x1004

#define GL_DONT_CARE 0x1100
#define GL_FASTEST   0x1101
#define GL_NICEST    0x1102

#define GL_BYTE           0x1400
#define GL_UNSIGNED_BYTE  0x1401
#define GL_SHORT          0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT            0x1404
#define GL_UNSIGNED_INT   0x1405
#define GL_FLOAT          0x1406

#define GL_CLEAR         0x1500
#define GL_AND           0x1501
#define GL_AND_REVERSE   0x1502
#define GL_COPY          0x1503
#define GL_AND_INVERTED  0x1504
#define GL_NOOP          0x1505
#define GL_XOR           0x1506
#define GL_OR            0x1507
#define GL_NOR           0x1508
#define GL_EQUIV         0x1509
#define GL_INVERT        0x150A
#define GL_OR_REVERSE    0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED   0x150D
#define GL_NAND          0x150E
#define GL_SET           0x150F

#define GL_TEXTURE 0x1702

#define GL_COLOR   0x1800
#define GL_DEPTH   0x1801
#define GL_STENCIL 0x1802

#define GL_STENCIL_INDEX   0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED             0x1903
#define GL_GREEN           0x1904
#define GL_BLUE            0x1905
#define GL_ALPHA           0x1906
#define GL_RGB             0x1907
#define GL_RGBA            0x1908

#define GL_POINT 0x1B00
#define GL_LINE  0x1B01
#define GL_FILL  0x1B02

#define GL_KEEP    0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR    0x1E02
#define GL_DECR    0x1E03

#define GL_VENDOR     0x1F00
#define GL_RENDERER   0x1F01
#define GL_VERSION    0x1F02
#define GL_EXTENSIONS 0x1F03

#define GL_NEAREST 0x2600
#define GL_LINEAR  0x2601

#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST  0x2701
#define GL_NEAREST_MIPMAP_LINEAR  0x2702
#define GL_LINEAR_MIPMAP_LINEAR   0x2703

#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S     0x2802
#define GL_TEXTURE_WRAP_T     0x2803

#define GL_REPEAT 0x2901

#define GL_DEPTH_BUFFER_BIT   0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT   0x00004000

#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE  0x2A02

#if !defined(APIENTRY) && defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif

#if defined(GL_UTILS_DEBUG_ENABLED)
#define glUtilsLogDebug(...) __glUtilsLogDebug(__FUNCTION__, __VA_ARGS__)
#else
#define glUtilsLogDebug(...)
#endif
#define glUtilsLogError(...) __glUtilsLogError(__FUNCTION__, __VA_ARGS__)

#if defined(__cplusplus)
extern "C" {
#endif

typedef char GLchar;
typedef signed char GLbyte;
typedef signed short int GLshort;
typedef signed short int GLhalf;
typedef int GLint;
typedef int GLsizei;
typedef float GLfloat;
typedef double GLdouble;

typedef unsigned char GLubyte;
typedef unsigned char GLboolean;
typedef unsigned short int GLushort;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;

typedef void (APIENTRY *PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY *PFNGLCLEARPROC)(GLbitfield mask);
typedef void (APIENTRY *PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY *PFNGLCLEARDEPTHPROC)(GLdouble depth);
typedef void (APIENTRY *PFNGLCLEARSTENCILPROC)(GLint s);
typedef void (APIENTRY *PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (APIENTRY *PFNGLCULLFACEPROC)(GLenum mode);
typedef void (APIENTRY *PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void (APIENTRY *PFNGLDEPTHMASKPROC)(GLboolean flag);
typedef void (APIENTRY *PFNGLDEPTHRANGEPROC)(GLdouble n, GLdouble f);
typedef void (APIENTRY *PFNGLDISABLEPROC)(GLenum cap);
typedef void (APIENTRY *PFNGLDRAWBUFFERPROC)(GLenum buf);
typedef void (APIENTRY *PFNGLENABLEPROC)(GLenum cap);
typedef void (APIENTRY *PFNGLFINISHPROC)(void);
typedef void (APIENTRY *PFNGLFLUSHPROC)(void);
typedef void (APIENTRY *PFNGLFRONTFACEPROC)(GLenum mode);
typedef void (APIENTRY *PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean *data);
typedef void (APIENTRY *PFNGLGETDOUBLEVPROC)(GLenum pname, GLdouble *data);
typedef GLenum (APIENTRY *PFNGLGETERRORPROC)(void);
typedef void (APIENTRY *PFNGLGETFLOATVPROC)(GLenum pname, GLfloat *data);
typedef void (APIENTRY *PFNGLGETINTEGERVPROC)(GLenum pname, GLint *data);
typedef const GLubyte* (APIENTRY *PFNGLGETSTRINGPROC)(GLenum name);
typedef void (APIENTRY *PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
typedef void (APIENTRY *PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (APIENTRY *PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRY *PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLHINTPROC)(GLenum target, GLenum mode);
typedef GLboolean (APIENTRY *PFNGLISENABLEDPROC)(GLenum cap);
typedef void (APIENTRY *PFNGLLINEWIDTHPROC)(GLfloat width);
typedef void (APIENTRY *PFNGLLOGICOPPROC)(GLenum opcode);
typedef void (APIENTRY *PFNGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
typedef void (APIENTRY *PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLPOINTSIZEPROC)(GLfloat size);
typedef void (APIENTRY *PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
typedef void (APIENTRY *PFNGLREADBUFFERPROC)(GLenum src);
typedef void (APIENTRY *PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
typedef void (APIENTRY *PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY *PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
typedef void (APIENTRY *PFNGLSTENCILMASKPROC)(GLuint mask);
typedef void (APIENTRY *PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void (APIENTRY *PFNGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY *PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY *PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);

typedef void (APIENTRY *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (APIENTRY *PFNGLCOPYTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (APIENTRY *PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (APIENTRY *PFNGLCOPYTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (APIENTRY *PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef void (APIENTRY *PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY *PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRY *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef GLboolean (APIENTRY *PFNGLISTEXTUREPROC)(GLuint texture);
typedef void (APIENTRY *PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
typedef void (APIENTRY *PFNGLTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);

typedef void (APIENTRY *PFNGLCOPYTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY *PFNGLDRAWRANGEELEMENTSPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRY *PFNGLTEXIMAGE3DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);

typedef void (APIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (APIENTRY *PFNGLCOMPRESSEDTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
typedef void (APIENTRY *PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (APIENTRY *PFNGLCOMPRESSEDTEXIMAGE3DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (APIENTRY *PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (APIENTRY *PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (APIENTRY *PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (APIENTRY *PFNGLGETCOMPRESSEDTEXIMAGEPROC)(GLenum target, GLint level, void *img);
typedef void (APIENTRY *PFNGLSAMPLECOVERAGEPROC)(GLfloat value, GLboolean invert);

typedef void (APIENTRY *PFNGLBLENDCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY *PFNGLBLENDEQUATIONPROC)(GLenum mode);
typedef void (APIENTRY *PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (APIENTRY *PFNGLMULTIDRAWARRAYSPROC)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
typedef void (APIENTRY *PFNGLMULTIDRAWELEMENTSPROC)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount);
typedef void (APIENTRY *PFNGLPOINTPARAMETERFPROC)(GLenum pname, GLfloat param);
typedef void (APIENTRY *PFNGLPOINTPARAMETERFVPROC)(GLenum pname, const GLfloat *params);
typedef void (APIENTRY *PFNGLPOINTPARAMETERIPROC)(GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLPOINTPARAMETERIVPROC)(GLenum pname, const GLint *params);

typedef void (APIENTRY *PFNGLGENQUERIESPROC)(GLsizei n, GLuint *ids);

/* OpenGL 1.0+ */
extern PFNGLBLENDFUNCPROC glBlendFunc;
extern PFNGLCLEARPROC glClear;
extern PFNGLCLEARCOLORPROC glClearColor;
extern PFNGLCLEARDEPTHPROC glClearDepth;
extern PFNGLCLEARSTENCILPROC glClearStencil;
extern PFNGLCOLORMASKPROC glColorMask;
extern PFNGLCULLFACEPROC glCullFace;
extern PFNGLDEPTHFUNCPROC glDepthFunc;
extern PFNGLDEPTHMASKPROC glDepthMask;
extern PFNGLDEPTHRANGEPROC glDepthRange;
extern PFNGLDISABLEPROC glDisable;
extern PFNGLDRAWBUFFERPROC glDrawBuffer;
extern PFNGLENABLEPROC glEnable;
extern PFNGLFINISHPROC glFinish;
extern PFNGLFLUSHPROC glFlush;
extern PFNGLFRONTFACEPROC glFrontFace;
extern PFNGLGETBOOLEANVPROC glGetBooleanv;
extern PFNGLGETDOUBLEVPROC glGetDoublev;
extern PFNGLGETERRORPROC glGetError;
extern PFNGLGETFLOATVPROC glGetFloatv;
extern PFNGLGETINTEGERVPROC glGetIntegerv;
extern PFNGLGETSTRINGPROC glGetString;
extern PFNGLGETTEXIMAGEPROC glGetTexImage;
extern PFNGLGETTEXLEVELPARAMETERFVPROC glGetTexLevelParameterfv;
extern PFNGLGETTEXLEVELPARAMETERIVPROC glGetTexLevelParameteriv;
extern PFNGLGETTEXPARAMETERFVPROC glGetTexParameterfv;
extern PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv;
extern PFNGLHINTPROC glHint;
extern PFNGLISENABLEDPROC glIsEnabled;
extern PFNGLLINEWIDTHPROC glLineWidth;
extern PFNGLLOGICOPPROC glLogicOp;
extern PFNGLPIXELSTOREFPROC glPixelStoref;
extern PFNGLPIXELSTOREIPROC glPixelStorei;
extern PFNGLPOINTSIZEPROC glPointSize;
extern PFNGLPOLYGONMODEPROC glPolygonMode;
extern PFNGLREADBUFFERPROC glReadBuffer;
extern PFNGLREADPIXELSPROC glReadPixels;
extern PFNGLSCISSORPROC glScissor;
extern PFNGLSTENCILFUNCPROC glStencilFunc;
extern PFNGLSTENCILMASKPROC glStencilMask;
extern PFNGLSTENCILOPPROC glStencilOp;
extern PFNGLTEXIMAGE1DPROC glTexImage1D;
extern PFNGLTEXIMAGE2DPROC glTexImage2D;
extern PFNGLTEXPARAMETERFPROC glTexParameterf;
extern PFNGLTEXPARAMETERFVPROC glTexParameterfv;
extern PFNGLTEXPARAMETERIPROC glTexParameteri;
extern PFNGLTEXPARAMETERIVPROC glTexParameteriv;
extern PFNGLVIEWPORTPROC glViewport;

/* OpenGL 1.1+ */
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLCOPYTEXIMAGE1DPROC glCopyTexImage1D;
extern PFNGLCOPYTEXIMAGE2DPROC glCopyTexImage2D;
extern PFNGLCOPYTEXSUBIMAGE1DPROC glCopyTexSubImage1D;
extern PFNGLCOPYTEXSUBIMAGE2DPROC glCopyTexSubImage2D;
extern PFNGLDELETETEXTURESPROC glDeleteTextures;
extern PFNGLDRAWARRAYSPROC glDrawArrays;
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLGENTEXTURESPROC glGenTextures;
extern PFNGLISTEXTUREPROC glIsTexture;
extern PFNGLPOLYGONOFFSETPROC glPolygonOffset;
extern PFNGLTEXSUBIMAGE1DPROC glTexSubImage1D;
extern PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;

/* OpenGL 1.2+ */
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
extern PFNGLTEXIMAGE3DPROC glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;

/* OpenGL 1.3+ */
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
extern PFNGLSAMPLECOVERAGEPROC glSampleCoverage;

/* OpenGL 1.4+ */
extern PFNGLBLENDCOLORPROC glBlendColor;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
extern PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
extern PFNGLPOINTPARAMETERFPROC glPointParameterf;
extern PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
extern PFNGLPOINTPARAMETERIPROC glPointParameteri;
extern PFNGLPOINTPARAMETERIVPROC glPointParameteriv;

/* OpenGL 1.5+ */
extern PFNGLGENQUERIESPROC glGenQueries;

void __glUtilsLogDebug(const char *functionName, const char *format, ...);
void __glUtilsLogError(const char *functionName, GLenum error, const char *format, ...);

const GLchar *glUtilsGetErrorName(GLenum error);
const GLchar *glUtilsGetErrorDescription(GLenum error);

typedef struct {
    int major;
    int minor;
} GlUtilsLibraryVersion;

typedef void *(*GlUtilsGetSymbolAddress)(const char *name);
GLboolean glUtilsLoadSymbols(GlUtilsGetSymbolAddress getSymbolAddress, GlUtilsLibraryVersion *version);

#if defined(__cplusplus)
}
#endif

#endif /* !_RHI_GL_UTILS_H */
