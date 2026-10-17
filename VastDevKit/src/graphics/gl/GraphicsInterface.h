#ifndef _VAST_GRAPHICS_INTERFACE_H_
#define _VAST_GRAPHICS_INTERFACE_H_

#ifdef WIN32
	#include <windows.h>
	#include <glew/glew.h>

	#define hdglMaxTexSize 8192
#elif TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
	#define GL_QUADS 888
	#define hdglMaxTexSize 1024

	typedef double GLdouble;
#elif defined(ANDROID)
    #include <GLES3/gl3platform.h>
    #include <GLES3/gl3.h>
    #include <GLES3/gl3ext.h>
    #define GL_QUADS 888
    #define hdglMaxTexSize 1024
    typedef double GLdouble;
#elif TARGET_OS_MAC == 1
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#define hdglMaxTexSize 8192
#else
	#warning "no gl directive found - including opengl by default" 
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>

	#define hdglMaxTexSize 8192
#endif

#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/AABB.h"
#include "math/Color4f.h"
#include "graphics/Texture/Texture.h"

#define BUFFER_OFFSET(i) ((void*)(i))

#define hdglError(_hdglUserinfo__) hd::gl::Error(_hdglUserinfo__, __FILE__, __LINE__, __FUNCTION__)

namespace hd {
    class gl {
    public:
        static void InitImmediateModeGL();
        
        static void ResetArrayPointers();
        
        static void Begin(GLenum prim);
        
        static void Ortho (GLdouble left, GLdouble right,
                           GLdouble bottom, GLdouble top,
                           GLdouble zNear, GLdouble zFar);
        
        static void Orthof(float left, float right,
                           float bottom, float top,
                           float zNear, float zFar);
        
        static void ClipPlane(GLenum plane, const GLdouble *equation);
        
        static void ClipPlanef(GLenum plane, const float *equation);

        static void Viewport(const AABB &aabb);

        static void Scissor(const AABB &aabb);

        static void BindTexture(const hd::Texture *texture);
        
        static void Vertex3f(float x, float y, float z);
        
        static void Vertex3f(const Vec3 &vec);
        
        static void Vertex2f(float x, float y);
        
        static void Vertex2f(const Vec2 &vec);
        
        static void Vertex2i(GLint x, GLint y);
        
        static void Color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
        
        static void Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        
        static void Color4f(const ::Color4f &color);
        
        static void TexCoord2i(GLint s, GLint t);
        
        static void TexCoord2f(GLfloat s, GLfloat t);
        
        static void TexCoord2f(const Vec2 &vec);
        
        static void End();

        static void EnableFaceCulling();

        static void DisableFaceCulling();
        
        static void ClearBuffers();
        
        static bool Error(const char *userInfo, const char *file, const int line, const char *func);
        
        static void BindTextureFrameBuffer(const hd::Texture *texture);
        
        static void PushActiveTexture();
        
        static void PopActiveTexture();
        
        static void ResetActiveTexture();
        
        static unsigned GetActiveTextureIndex();
        
        static GLenum WrapToGL(TextureWrapMode mode);
        
        static GLenum MagFilterToGL(TextureMagFilter MagFilter);
        
        static GLenum MinFilterToGL(bool MipMap, TextureMinFilter MinFilter);
        
    };
}

#endif