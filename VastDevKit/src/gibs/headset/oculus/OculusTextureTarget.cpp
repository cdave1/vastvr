#include "OculusTextureTarget.h"
#include "math/Geometry.h"
#include "graphics/components/Shader.h"
#include "core/device/Device.h"


hd::input::OculusTextureTarget::OculusTextureTarget() :
m_frameBufferId(0),
m_textureSwapChain(0),
m_depthBufferTextureId(0),
m_size() {
}


void hd::input::OculusTextureTarget::Create(ovrSession hmd, const Size2 &size) {
	m_size = size;

	//OVR_ASSERT(hmd); // No HMD? A little odd.

	// https://developer3.oculus.com/documentation/pcsdk/latest/concepts/dg-render/#dg-render-concepts
	ovrTextureSwapChainDesc desc = {};
	desc.Type = ovrTexture_2D;
	desc.ArraySize = 1;
	desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.Width = size[0];
	desc.Height = size[1];
	desc.MipLevels = 1;
	desc.SampleCount = 1;
	desc.StaticImage = ovrFalse;

	ovrResult result = ovr_CreateTextureSwapChainGL(hmd, &desc, &m_textureSwapChain);

	int length;
	ovr_GetTextureSwapChainLength(hmd, m_textureSwapChain, &length);

	if (OVR_SUCCESS(result)) {
		for (int i = 0; i < length; ++i) {
			unsigned int texId;

			ovr_GetTextureSwapChainBufferGL(hmd, m_textureSwapChain, i, &texId);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
	} else {
		log::w("Could not create texture swap chain");
		return;
	}

	glGenFramebuffers(1, &m_frameBufferId);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		hd::log::e("framebuffer not complete: %x", status);
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	// Depth buffer stuff
	glGenTextures(1, &m_depthBufferTextureId);
	glBindTexture(GL_TEXTURE_2D, m_depthBufferTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLenum internalFormat = GL_DEPTH_COMPONENT24;
	GLenum type = GL_UNSIGNED_INT;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size[0], size[1], 0, GL_DEPTH_COMPONENT, type, NULL);
}


void hd::input::OculusTextureTarget::Clear() {
    //glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}


void hd::input::OculusTextureTarget::Start(ovrSession hmd) {
	int index;
	ovr_GetTextureSwapChainCurrentIndex(hmd, m_textureSwapChain, &index);

	unsigned int texId;
	ovr_GetTextureSwapChainBufferGL(hmd, m_textureSwapChain, index, &texId);

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBufferTextureId, 0);

    glViewport(0, 0, m_size[0], m_size[1]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_FRAMEBUFFER_SRGB);

	hdglError(__FUNCTION__);
}


void hd::input::OculusTextureTarget::End() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	hdglError(__FUNCTION__);
}


ovrTextureSwapChain hd::input::OculusTextureTarget::GetOvrTextureSwapChain() const {
    return m_textureSwapChain;
}


Size2 hd::input::OculusTextureTarget::GetSize() const {
    return m_size;
}
