#include "GeometryRenderTarget.h"
#include "math/Geometry.h"
#include "graphics/components/Shader.h"
#include "core/device/Device.h"

using namespace hd::graphics;

GeometryRenderTarget::GeometryRenderTarget(unsigned int ID) : Component(ID),
m_frameBufferId(0),
m_depthBufferId(0),
m_clearColor(0.0f, 0.0f, 0.0f, 1.0f),
m_size(),
m_colorTexture(NULL),
m_lightingTexture(NULL),
m_normalsTexture(NULL),
m_depthTexture(NULL),
m_renderCallback() {
    Reset();
}


void GeometryRenderTarget::Reset() {
    if (m_frameBufferId != 0) {
        glDeleteFramebuffers(1, &m_frameBufferId);
        m_frameBufferId = 0;
    }
    
    if (m_depthBufferId != 0) {
        glDeleteRenderbuffers(1, &m_depthBufferId);
        m_depthBufferId = 0;
    }
    
    if (m_colorTexture) {
        TextureManager::Instance()->DestroyTexture(m_colorTexture);
        m_colorTexture = NULL;
    }

    if (m_lightingTexture) {
        TextureManager::Instance()->DestroyTexture(m_lightingTexture);
        m_lightingTexture = NULL;
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


void GeometryRenderTarget::Clear() {
    //glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void GeometryRenderTarget::ClearColor() {
    //glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}


void GeometryRenderTarget::ClearDepth() {
    //glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_DEPTH_BUFFER_BIT);
}


void GeometryRenderTarget::SetDepthMask() {
    glDepthMask(GL_TRUE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}


void GeometryRenderTarget::SetColorMask() {
    glDepthMask(GL_FALSE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}


void GeometryRenderTarget::Resize(const Size2 &size) {
    Reset();
    
    hdAssert(m_colorTexture == NULL);
    hdAssert(m_depthTexture == NULL);
    hdAssert(m_normalsTexture == NULL);
    hdAssert(m_frameBufferId == 0);
    
    m_size = size;
    
    glGenFramebuffers(1, &m_frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    
    m_colorTexture = TextureManager::Instance()->GenerateRenderTexture(size);
    m_lightingTexture = TextureManager::Instance()->GenerateRenderTexture(size);
    m_normalsTexture = TextureManager::Instance()->GenerateRenderTexture(size);
    m_depthTexture = TextureManager::Instance()->GenerateDepthBuffer(size);
    
    glGenRenderbuffers(1, &m_depthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferId);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size[0], m_size[1]);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture->texnum, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightingTexture->texnum, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_normalsTexture->texnum, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->texnum, 0);

#ifdef DEBUG
    int32_t depthSize, stencilSize, componentType;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depthSize);
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencilSize);
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE, &componentType);
    hd::log::i("framebuffer depth size: %d, stencil size: %d, component type: %x\n", depthSize, stencilSize, componentType);
#endif

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer not complete: %x", status);
    }
    
    End();
}


void GeometryRenderTarget::SetClearColor(const Color4f &color) {
    m_clearColor = color;
}


void GeometryRenderTarget::Start() {
    hdAssert(m_size[0] > 0 && m_size[1] > 0);

    if (m_colorTexture == NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    
    const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, buffers);

    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glViewport(0, 0, m_size[0], m_size[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer not complete: %x", status);
    }
}


void GeometryRenderTarget::End() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer error: %x", status);
    }
}


void GeometryRenderTarget::StartDepthOnly() {
    hdAssert(m_size[0] > 0 && m_size[1] > 0);

    if (m_colorTexture == NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer not complete: %x", status);
    }
}


const hd::Texture * GeometryRenderTarget::GetColorTexture() const {
    return m_colorTexture;
}


const hd::Texture * GeometryRenderTarget::GetLightingTexture() const {
    return m_lightingTexture;
}


const hd::Texture * GeometryRenderTarget::GetNormalsTexture() const {
    return m_normalsTexture;
}


const hd::Texture * GeometryRenderTarget::GetDepthTexture() const {
    return m_depthTexture;
}


Size2 GeometryRenderTarget::GetTextureSize() const {
    return m_size;
}


void GeometryRenderTarget::Render(const hd::Entity *entity) {
    if (m_renderCallback) {
        m_renderCallback(entity);
    }
}
