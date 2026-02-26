/*
 * OpenGL definitions.
 * Copyright (c) 2025-2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#define __gl_h_ 1

#define GL_FALSE 0
#define GL_TRUE  1

#define GL_NO_ERROR 0
#define GL_INVALID_ENUM                  0x0500
#define GL_INVALID_VALUE                 0x0501
#define GL_INVALID_OPERATION             0x0502
#define GL_STACK_OVERFLOW                0x0503
#define GL_STACK_UNDERFLOW               0x0504
#define GL_OUT_OF_MEMORY                 0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_CONTEXT_LOST                  0x0507

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1

#define GL_BYTE           0x1400
#define GL_UNSIGNED_BYTE  0x1401
#define GL_SHORT          0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT            0x1404
#define GL_UNSIGNED_INT   0x1405
#define GL_FLOAT          0x1406
#define GL_DOUBLE         0x140A
#define GL_HALF_FLOAT     0x140B
#define GL_FIXED          0x140C

#define GL_STENCIL_INDEX   0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED             0x1903
#define GL_GREEN           0x1904
#define GL_BLUE            0x1905
#define GL_ALPHA           0x1906
#define GL_RGB             0x1907
#define GL_RGBA            0x1908

#define GL_VENDOR     0x1F00
#define GL_RENDERER   0x1F01
#define GL_VERSION    0x1F02
#define GL_EXTENSIONS 0x1F03

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER   0x8B31

#define GL_DELETE_STATUS                   0x8B80
#define GL_COMPILE_STATUS                  0x8B81
#define GL_LINK_STATUS                     0x8B82
#define GL_VALIDATE_STATUS                 0x8B83
#define GL_INFO_LOG_LENGTH                 0x8B84
#define GL_ATTACHED_SHADERS                0x8B85
#define GL_ACTIVE_UNIFORMS                 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH       0x8B87
#define GL_SHADER_SOURCE_LENGTH            0x8B88
#define GL_ACTIVE_ATTRIBUTES               0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH     0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION        0x8B8C
#define GL_CURRENT_PROGRAM                 0x8B8D

#if !defined(APIENTRY) && defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
    #define APIENTRY __stdcall
#else
    #define APIENTRY
#endif

#if defined(__cplusplus)
extern "C" {
#endif

typedef char         GLchar;
typedef signed char  GLbyte;
typedef signed short GLshort;
typedef signed short GLhalf;
typedef int          GLint;
typedef int          GLsizei;
typedef float        GLfloat;
typedef double       GLdouble;

typedef unsigned char  GLubyte;
typedef unsigned char  GLboolean;
typedef unsigned short GLushort;
typedef unsigned int   GLuint;
typedef unsigned int   GLenum;
typedef unsigned int   GLbitfield;

typedef signed long GLintptr;
typedef signed long GLsizeiptr;

typedef GLenum (APIENTRY *PFNGLGETERRORPROC)(void);
typedef const GLubyte *(APIENTRY *PFNGLGETSTRINGPROC)(GLenum name);

#if defined(__cplusplus)
}
#endif
