#include "RenderTarget.h"
#include "math/Geometry.h"
#include "graphics/Components/Shader.h"
#include "core/device/Device.h"

using namespace hd::graphics;

RenderTarget::RenderTarget(unsigned int ID) : Component(ID),
    m_frameBufferId(0),
    m_depthBufferId(0),
    m_clearColor(0.0f, 0.0f, 0.0f, 0.0f),
    m_size(),
    m_texture(NULL),
    m_normalsTexture(NULL),
    m_depthTexture(NULL),
    m_renderGeometryCallback() {
    Reset();
}


void RenderTarget::Reset() {
    if (m_frameBufferId != 0) {
        glDeleteFramebuffers(1, &m_frameBufferId);
        m_frameBufferId = 0;
    }
    
    if (m_depthBufferId != 0) {
        glDeleteRenderbuffers(1, &m_depthBufferId);
        m_depthBufferId = 0;
    }
    
    if (m_texture) {
        TextureManager::Instance()->DestroyTexture(m_texture);
        m_texture = NULL;
    }
    
    if (m_normalsTexture) {
        TextureManager::Instance()->DestroyTexture(m_normalsTexture);
        m_normalsTexture = NULL;
    }
    
    if (m_depthTexture) {
        TextureManager::Instance()->DestroyTexture(m_depthTexture);
        m_depthTexture = NULL;
    }
}


void RenderTarget::Clear() {
    //glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


static bool m_depthTextureAsRenderTexture = false;
void RenderTarget::Resize(const Size2 &size) {
    Reset();
    
    assert(m_texture == NULL);
    assert(m_depthTexture == NULL);
    assert(m_frameBufferId == 0);
    
    m_size = size;
    
    glGenFramebuffers(1, &m_frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    
    if (m_depthTextureAsRenderTexture) {
        m_texture = TextureManager::Instance()->GenerateRenderTexture(size);
        m_normalsTexture = TextureManager::Instance()->GenerateRenderTexture(size);
        m_depthTexture = TextureManager::Instance()->GenerateRenderTexture(size);
        Texture *tmp = TextureManager::Instance()->GenerateDepthBuffer(size);
        
        glGenRenderbuffers(1, &m_depthBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size[0], m_size[1]);
        //glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->texnum, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalsTexture->texnum, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_depthTexture->texnum, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tmp->texnum, 0);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->texnum, 0);
    } else {
        m_texture = TextureManager::Instance()->GenerateRenderTexture(size);
        m_normalsTexture = TextureManager::Instance()->GenerateRenderTexture(size);
        m_depthTexture = TextureManager::Instance()->GenerateDepthBuffer(size);
        
        glGenRenderbuffers(1, &m_depthBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size[0], m_size[1]);
        //glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->texnum, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalsTexture->texnum, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->texnum, 0);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->texnum, 0);
    }
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer not complete: %x", status);
    }
    
    End();
}


void RenderTarget::SetClearColor(const Color4f &color) {
    m_clearColor = color;
}


void RenderTarget::Start() {
    hdAssert(m_size[0] > 0 && m_size[1] > 0);

    if (m_texture == NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->texnum, 0);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalsTexture->texnum, 0);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->texnum, 0);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->texnum, 0);
    if (m_depthTextureAsRenderTexture) {
        const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, buffers);

		glViewport(0, 0, m_size[0], m_size[1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            hd::log::e("framebuffer not complete: %x", status);
        }
        
        //hd::gl::PushActiveTexture();
    } else {
        const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, buffers);
        
		glViewport(0, 0, m_size[0], m_size[1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            hd::log::e("framebuffer not complete: %x", status);
        }
        
        //hd::gl::PushActiveTexture();
    }
}


void RenderTarget::End() {
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer error: %x", status);
    }
}


const hd::Texture * RenderTarget::GetTexture() const {
    return m_texture;
}


const hd::Texture * RenderTarget::GetNormalsTexture() const {
    return m_normalsTexture;
}


const hd::Texture * RenderTarget::GetDepthTexture() const {
    return m_depthTexture;
}


void RenderTarget::BindTexturesToShader(Shader *shader,
                                        const std::string &colorSamplerName,
                                        const std::string &normalSamplerName,
                                        const std::string &depthSamplerName) {
    shader->BindTexture(colorSamplerName.c_str(), m_texture);
    shader->BindTexture(normalSamplerName.c_str(), m_normalsTexture);
    shader->BindTexture(depthSamplerName.c_str(), m_depthTexture);
}


Size2 RenderTarget::GetTextureSize() const {
    if (m_texture) return Size2(m_texture->width, m_texture->height);
    return Size2();
}


void RenderTarget::RenderGeometry(const hd::Entity *entity) {
    if (m_renderGeometryCallback) {
        m_renderGeometryCallback(entity);
    }
}
