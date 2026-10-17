#include "OpacityBlock.h"
#include <Vast/VastMath.h>

#include "graphics/gl/GraphicsInterface.h"

hd::animation::OpacityBlock::OpacityBlock(unsigned int ID) : Component(ID),
    m_startOpacity(1.0f),
    m_endOpacity(1.0f),
    m_isEnabled(true)
{
    Reset();
}


void hd::animation::OpacityBlock::Reset() {
    m_startOpacity = 1.0f;
    m_endOpacity = 1.0f;
    m_isEnabled = true;
}


void hd::animation::OpacityBlock::Reverse() {
    float tmp = m_endOpacity;
    m_endOpacity = m_startOpacity;
    m_startOpacity = tmp;
}


void hd::animation::OpacityBlock::Enable() {
    m_isEnabled = true;
}


void hd::animation::OpacityBlock::Disable() {
    m_isEnabled = false;
}


bool hd::animation::OpacityBlock::IsEnabled() const {
    return m_isEnabled;
}


void hd::animation::OpacityBlock::SetStartOpacity(const float opacity) {
    m_startOpacity = opacity;
}


void hd::animation::OpacityBlock::SetEndOpacity(const float opacity) {
    m_endOpacity = opacity;
}


float hd::animation::OpacityBlock::GetOpacity(const float timer) const {
    return hd::lerp(m_startOpacity, m_endOpacity, timer);
}
