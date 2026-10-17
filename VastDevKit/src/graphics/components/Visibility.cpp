#include "Visibility.h"
#include "math/Geometry.h"

using namespace hd::graphics;

Visibility::Visibility(unsigned int ID) : Component(ID),
    m_currentFrame(0) {
    Reset();
}


void Visibility::Reset() {
    m_currentFrame = 0;
}


void Visibility::SetCurrentFrame(const uint32_t &frame) {
    m_currentFrame = frame;
}


const uint32_t Visibility::GetCurrentFrame() const {
    return m_currentFrame;
}