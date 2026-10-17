#include "GraphicsInterface.h"

void hd::gl::InitImmediateModeGL() {}


void hd::gl::ResetArrayPointers() {}


void hd::gl::ClearBuffers() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void hd::gl::Begin(GLenum prim) {
	//glBegin(prim);
}


void hd::gl::Ortho (GLdouble left, GLdouble right,
                    GLdouble bottom, GLdouble top,
                    GLdouble zNear, GLdouble zFar) {
	//glOrtho(left, right, bottom, top, zNear, zFar);
}


void hd::gl::Orthof(float left, float right,
                    float bottom, float top,
                    float zNear, float zFar) {
	//glOrtho((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar);
}


void hd::gl::ClipPlane(GLenum plane, const GLdouble *equation) {
	//glClipPlane(plane, equation);
}


void hd::gl::ClipPlanef(GLenum plane, const float *equation) {
//	GLdouble eqn[] = {equation[0], equation[1], equation[2], equation[3]};
	
	//glClipPlane(plane, eqn);
}


void hd::gl::Viewport(const AABB &aabb) {
    glViewport(aabb.lower[0], aabb.lower[1],
               aabb.Width(), aabb.Height());
}


void hd::gl::Scissor(const AABB &aabb) {
    glEnable(GL_SCISSOR_TEST);
    glScissor(aabb.lower[0], aabb.lower[1],
              aabb.Width(), aabb.Height());
}


static GLint activeTextureIndex = 0;
static int currentTexNum = -1;
void hd::gl::BindTexture(const hd::Texture *texture) {
    if (hdglError("")) {
        verr("");
    }
    glActiveTexture(GL_TEXTURE0 + activeTextureIndex);
	if (texture == NULL) {
		glBindTexture(GL_TEXTURE_2D, 0);
        currentTexNum = -1;
        return;
    }
    
    if (texture->texnum != currentTexNum) {
        currentTexNum = texture->texnum;
        glBindTexture(GL_TEXTURE_2D, texture->texnum);
        hdglError("glBindTexture");
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL(texture->WrapS));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL(texture->WrapT));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL(texture->MipMap, texture->MinFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL(texture->MagFilter));
        
        hdglError("glTexParameterf");
    }
}


void hd::gl::PushActiveTexture() {
    if (activeTextureIndex > 100)
    {
        hd::log::e("Holy shit!");
    }
    activeTextureIndex++;
}


void hd::gl::PopActiveTexture() {
    if (activeTextureIndex > 0) {
        activeTextureIndex--;
    }
}


void hd::gl::ResetActiveTexture() {
    activeTextureIndex = 0;
    glBindTexture(GL_TEXTURE_2D, 0);
}


unsigned hd::gl::GetActiveTextureIndex() {
    return activeTextureIndex;
}


void hd::gl::Vertex3f(float x, float y, float z) {
	//glVertex3f(x, y, z);
}


void hd::gl::Vertex3f(const Vec3 &vec) {
    //glVertex3f(vec[0], vec[1], vec[2]);
}


void hd::gl::Vertex2i(GLint x, GLint y) {
	//glVertex2i(x, y);
}


void hd::gl::Vertex2f(float x, float y) {
	//glVertex2f(x, y);
}


void hd::gl::Vertex2f(const Vec2 &vec) {
    //glVertex2f(vec[0], vec[1]);
}


void hd::gl::Color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
	//glColor4ub(r, g, b, a);
}


void hd::gl::Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	//glColor4f(r, g, b, a);
}


void hd::gl::Color4f(const ::Color4f &color) {
    //glColor4f(color[0], color[1], color[2], color[3]);
}


void hd::gl::TexCoord2i(GLint s, GLint t) {
	//glTexCoord2i(s, t);
}


void hd::gl::TexCoord2f(GLfloat s, GLfloat t) {
    //	glTexCoord2i(s, t);
	//glTexCoord2f(s, t);
}


void hd::gl::TexCoord2f(const Vec2 &vec) {
    hd::gl::TexCoord2f(vec[0], vec[1]);
}


void hd::gl::End() {
	//glEnd();
}


void hd::gl::EnableFaceCulling() {
    glEnable(GL_CULL_FACE);
}


void hd::gl::DisableFaceCulling() {
    glDisable(GL_CULL_FACE);
}


GLenum hd::gl::WrapToGL(TextureWrapMode mode) {
	if (mode == TextureWrapMode::Repeat) {
		return GL_REPEAT;
	} else if (mode == TextureWrapMode::ClampToBorder) {
#ifdef GL_CLAMP_TO_BORDER
        return GL_CLAMP_TO_BORDER;
#else
        return GL_CLAMP_TO_EDGE;
#endif
    } else {
		return GL_CLAMP_TO_EDGE;
	}
}


GLenum hd::gl::MagFilterToGL(TextureMagFilter MagFilter) {
	switch (MagFilter) {
		case TextureMagFilter::Nearest:
			return GL_NEAREST;
			
		case TextureMagFilter::Linear:
			return GL_LINEAR;
			
		default:
			break;
	}
	
	return GL_LINEAR;
}


GLenum hd::gl::MinFilterToGL(bool MipMap, TextureMinFilter MinFilter) {
	if (MipMap) {
		switch (MinFilter) {
			case TextureMinFilter::NearestMipMapOff:
				return GL_NEAREST;
				
			case TextureMinFilter::NearestMipMapNearest:
				return GL_NEAREST_MIPMAP_NEAREST;
				
			case TextureMinFilter::NearestMipMapLinear:
				return GL_NEAREST_MIPMAP_LINEAR;
				
			case TextureMinFilter::LinearMipMapOff:
				return GL_LINEAR;
				
			case TextureMinFilter::LinearMipMapNearest:
				return GL_LINEAR_MIPMAP_NEAREST;
				
			case TextureMinFilter::LinearMipMapLinear:
				return GL_LINEAR_MIPMAP_LINEAR;
				
			default:
				break;
		}
	} else {
		switch (MinFilter) {
			case TextureMinFilter::NearestMipMapOff:
			case TextureMinFilter::NearestMipMapNearest:
			case TextureMinFilter::NearestMipMapLinear:
				return GL_NEAREST;
				
			case TextureMinFilter::LinearMipMapOff:
			case TextureMinFilter::LinearMipMapNearest:
			case TextureMinFilter::LinearMipMapLinear:
				return GL_LINEAR;
				
			default:
				break;
		}
	}
	
	return GL_LINEAR;
}


bool hd::gl::Error(const char *userInfo, const char *file, const int line, const char *func) {
	GLenum error = glGetError();
	bool ret = false;
    
    while (error != GL_NO_ERROR) {
        const char *errorString;
        
        switch (error) {
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                errorString = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            //case GL_STACK_OVERFLOW:
            //    errorString = "GL_STACK_OVERFLOW";
            //    break;
            //case GL_STACK_UNDERFLOW:
            //    errorString = "GL_STACK_UNDERFLOW";
            //    break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            default:
                errorString = "UNHANDLED ERRORCODE";
                break;
        }
        ret = true;
        hd::log::i("[GL ERROR] %s %x (at %s in %s:%d) %s\n", errorString, error, func, file, line, userInfo);
        error = glGetError();
    }
	return ret;
}
