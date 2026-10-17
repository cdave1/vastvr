#include "PixelBufferObject.h"
#include "graphics/gl/GraphicsInterface.h"
#include "graphics/texture/PNGLoader.h"

using namespace hd::graphics;


PixelBufferObject::PixelBufferObject() :
m_size(),
m_pixelCount(0),
m_colorPixelSize(sizeof(uint32_t)),
m_colorCopyBuffer(NULL),
m_colorPixelBufferIds(),
m_totalColorPixelBuffers(MAX_PIXEL_BUFFER_OBJECTS),
m_colorVram2sys(0),
m_colorGpu2vram(0) {
    Reset();
}


PixelBufferObject::~PixelBufferObject() {
    Reset();
}


void PixelBufferObject::Reset() {
    m_size.Set(0, 0);
    m_pixelCount = 0;

    glDeleteBuffers(m_totalColorPixelBuffers, m_colorPixelBufferIds);
    for (uint32_t i = 0; i < m_totalColorPixelBuffers; ++i) {
        m_colorPixelBufferIds[i] = 0;
    }
    m_colorVram2sys = 0;
    m_colorGpu2vram = 0;

    if (m_colorCopyBuffer) {
        hdFreeSafely(m_colorCopyBuffer);
    }
}


void PixelBufferObject::SetSize(const Size2 &size) {
    Reset();
    
    m_size = size;
    m_pixelCount = m_size[0] * m_size[1];

    m_colorPixelSize = sizeof(uint8_t) * 4;
    if (!(m_colorCopyBuffer = (uint8_t *)calloc(1, m_colorPixelSize * m_pixelCount))) {
        hd::log::e("Could not allocate bytes for color pixel buffer\n");
        return;
    }
    glGenBuffers(m_totalColorPixelBuffers, m_colorPixelBufferIds);
    for (uint32_t i = 0; i < m_totalColorPixelBuffers; ++i) {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_colorPixelBufferIds[i]);
        glBufferData(GL_PIXEL_PACK_BUFFER, m_colorPixelSize * m_pixelCount, NULL, GL_STREAM_READ);
    }
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    m_colorVram2sys = 0;
    m_colorGpu2vram = m_totalColorPixelBuffers - 1;
}


void PixelBufferObject::Update() {
    hdAssert(m_size[0] > 0 && m_size[1] > 0);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_colorPixelBufferIds[m_colorGpu2vram]);
    glBufferData(GL_PIXEL_PACK_BUFFER, m_colorPixelSize * m_pixelCount, NULL, GL_STREAM_READ);

#ifdef GL_BGRA
    glReadPixels(0, 0, m_size[0], m_size[1], GL_BGRA, GL_UNSIGNED_BYTE, 0);
#else
    glReadPixels(0, 0, m_size[0], m_size[1], GL_RGBA, GL_UNSIGNED_BYTE, 0);
#endif
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_colorPixelBufferIds[m_colorVram2sys]);

    // TODO: reduce this range, we only need one pixel!
    uint8_t *data = (uint8_t *)glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, m_colorPixelSize * m_pixelCount, GL_MAP_READ_BIT);
    if (data) {
#if 0
        for (uint32_t i = 0; i < m_size[1]; ++i) {
            memcpy(m_colorCopyBuffer + ((m_size[1] - i) * m_size[0] * m_colorPixelSize), data + (i * m_size[0] * m_colorPixelSize), m_size[0] * m_colorPixelSize);
        }
#else
        memcpy(m_colorCopyBuffer, data, m_pixelCount * m_colorPixelSize);
#endif
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    }
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    uint32_t temp = m_colorPixelBufferIds[0];
    for (uint32_t i = 1; i < m_totalColorPixelBuffers; ++i) {
        m_colorPixelBufferIds[i - 1] = m_colorPixelBufferIds[i];
    }
    m_colorPixelBufferIds[m_totalColorPixelBuffers - 1] = temp;
}


Size2 PixelBufferObject::GetSize() const {
    return m_size;
}


Color4f PixelBufferObject::GetColor(const Vec2 &offset) const {
	uint32_t pos = uint32_t(m_pixelCount * 0.5f) + uint32_t(m_size[0] * 0.5f) + (int(offset[0]));
    pos *= m_colorPixelSize;

    // NOTE: Pixels are BGRA format.
    Color4f color;
#ifdef GL_BGRA
    color.Seti((int)m_colorCopyBuffer[pos + 2],
               (int)m_colorCopyBuffer[pos + 1],
               (int)m_colorCopyBuffer[pos + 0],
               (int)m_colorCopyBuffer[pos + 3]);
#else
    color.Seti((int)m_colorCopyBuffer[pos + 0],
               (int)m_colorCopyBuffer[pos + 1],
               (int)m_colorCopyBuffer[pos + 2],
               (int)m_colorCopyBuffer[pos + 3]);
#endif
    return color;
}
