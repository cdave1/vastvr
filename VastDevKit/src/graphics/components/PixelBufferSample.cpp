#include "PixelBufferSample.h"

using namespace hd;
using namespace hd::graphics;

PixelBufferSample::PixelBufferSample(unsigned int ID) : Component(ID),
    m_sampleColor(1.0f, 1.0f, 1.0f, 1.0f),
    m_uv(0.5f, 0.5f) {}


void PixelBufferSample::Reset() {
    m_sampleColor = Color4f(0.2f, 0.2f, 0.2f, 1.0f);
    m_uv = Vec2(0.5f, 0.5f);
}
