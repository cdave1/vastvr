#include "DepthRenderTarget.h"
#include "math/Geometry.h"
#include "graphics/Components/Shader.h"
#include "core/device/Device.h"

using namespace hd::graphics;

DepthRenderTarget::DepthRenderTarget(unsigned int ID) : Component(ID),
        m_frameBufferId(0),
        m_clearColor(0.0f, 0.0f, 0.0f, 1.0f),
        m_size(),
        m_depthTexture(NULL),
        m_renderGeometryCallback() {
    Reset();
}


void DepthRenderTarget::Reset() {
    if (m_frameBufferId != 0) {
        glDeleteFramebuffers(1, &m_frameBufferId);
        m_frameBufferId = 0;
    }
    
    if (m_depthTexture) {
        TextureManager::Instance()->DestroyTexture(m_depthTexture);
        m_depthTexture = NULL;
    }
}


void DepthRenderTarget::Clear() {
    //glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_DEPTH_BUFFER_BIT);
}


void DepthRenderTarget::Resize(const Size2 &size) {
    Reset();
    
    assert(m_depthTexture == NULL);
    assert(m_frameBufferId == 0);
    
    m_size = size;
    
    m_depthTexture = TextureManager::Instance()->GenerateDepthBuffer(size);
    
    hd::gl::BindTexture(NULL);
    
    glGenFramebuffers(1, &m_frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->texnum, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer not complete: %x", status);
    }
    
    End();
}


void DepthRenderTarget::SetClearColor(const Color4f &color) {
    m_clearColor = color;
}


void DepthRenderTarget::Start() {
    hdAssert(m_size[0] > 0 && m_size[1] > 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

	glViewport(0, 0, m_size[0], m_size[1]);
	glClear(GL_DEPTH_BUFFER_BIT);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer not complete: %x", status);
    }
    //glDepthMask(GL_TRUE);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glCullFace(GL_FRONT);
}


void DepthRenderTarget::End() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer error: %x", status);
    }
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glCullFace(GL_BACK);
}


const hd::Texture * DepthRenderTarget::GetDepthTexture() const {
    return m_depthTexture;
}


void DepthRenderTarget::BindTexturesToShader(Shader *shader,
                                             const std::string &depthSamplerName) {
    shader->BindTexture(depthSamplerName.c_str(), m_depthTexture);
}


Size2 DepthRenderTarget::GetTextureSize() const {
    if (m_depthTexture) return Size2(m_depthTexture->width, m_depthTexture->height);
    return Size2();
}


void DepthRenderTarget::RenderGeometry(const hd::Entity *entity) {
    if (m_renderGeometryCallback) {
        m_renderGeometryCallback(entity);
    }
}
