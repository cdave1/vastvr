#include "TextureTarget.h"
#include "math/Geometry.h"
#include "graphics/components/Shader.h"
#include "core/device/Device.h"

using namespace hd::graphics;

TextureTarget::TextureTarget(unsigned int ID) : Component(ID),
m_frameBufferId(0),
m_clearColor(0.0f, 0.0f, 0.0f, 1.0f),
m_size(),
m_texture(NULL) {
    Reset();
}


void TextureTarget::Reset() {
    if (m_frameBufferId != 0) {
        glDeleteFramebuffers(1, &m_frameBufferId);
        m_frameBufferId = 0;
    }

    if (m_texture) {
        TextureManager::Instance()->DestroyTexture(m_texture);
        m_texture = NULL;
    }
}


void TextureTarget::Clear() {
    //glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}


void TextureTarget::Resize(const Size2 &size) {
    Reset();

    hdAssert(m_texture == NULL);
    hdAssert(m_frameBufferId == 0);

    m_size = size;

    glGenFramebuffers(1, &m_frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

    m_texture = TextureManager::Instance()->GenerateRenderTexture(size);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->texnum, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer not complete: %x", status);
    }

    End();
}


void TextureTarget::SetClearColor(const Color4f &color) {
    m_clearColor = color;
}


void TextureTarget::Start() {
    hdAssert(m_size[0] > 0 && m_size[1] > 0);

    if (m_texture == NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

    const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

	glViewport(0, 0, m_size[0], m_size[1]);
	glClear(GL_COLOR_BUFFER_BIT);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer not complete: %x", status);
    }
}


void TextureTarget::End() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        hd::log::e("framebuffer error: %x", status);
    }
}


const hd::Texture * TextureTarget::GetTexture() const {
    return m_texture;
}


Size2 TextureTarget::GetTextureSize() const {
    if (m_texture) return Size2(m_texture->width, m_texture->height);
    return Size2();
}
