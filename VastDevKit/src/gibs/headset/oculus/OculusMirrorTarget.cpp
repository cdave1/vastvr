#include "OculusMirrorTarget.h"
#include "math/Geometry.h"
#include "graphics/components/Shader.h"
#include "core/device/Device.h"


hd::input::OculusMirrorTarget::OculusMirrorTarget() :
m_frameBufferId(0),
m_mirrorTexture(nullptr),
m_size() {}


void hd::input::OculusMirrorTarget::Create(ovrSession hmd, const Size2 &size) {
	m_size = size;

	//OVR_ASSERT(hmd);

	ovrMirrorTextureDesc desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width = size[0];
	desc.Height = size[1];
	desc.Format = ovrTextureFormat::OVR_FORMAT_R8G8B8A8_UNORM_SRGB;//GL_SRGB8_ALPHA8;

	//ovrResult result = ovr_CreateMirrorTextureGL(hmd, GL_SRGB8_ALPHA8, size[0], size[1], reinterpret_cast<ovrTexture**>(&m_mirrorTexture));
	ovrResult result = ovr_CreateMirrorTextureGL(hmd, &desc, &m_mirrorTexture);
	if (!OVR_SUCCESS(result)) {
		hd::log::e("Failed to initialzise mirror texture (code: %d).", result);
	}

	unsigned int mirrorTextureBufferId;
	ovr_GetMirrorTextureBufferGL(hmd, m_mirrorTexture, &mirrorTextureBufferId);

	glGenFramebuffers(1, &m_frameBufferId);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTextureBufferId, 0);
	glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		hd::log::e("framebuffer not complete: %x", status);
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}


void hd::input::OculusMirrorTarget::Render() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GLint w = m_size[0];// m_mirrorTexture->OGL.Header.TextureSize.w;
	GLint h = m_size[1]; // m_mirrorTexture->OGL.Header.TextureSize.h;
	glBlitFramebuffer(0, h, w, 0, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}


ovrMirrorTexture hd::input::OculusMirrorTarget::GetOvrGLTexture() const {
	return m_mirrorTexture;
}


Size2 hd::input::OculusMirrorTarget::GetSize() const {
    return m_size;
}
