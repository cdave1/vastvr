#include "ScaleBlock.h"
#include "math/Geometry.h"
#include "graphics/gl/GraphicsInterface.h"

hd::animation::ScaleBlock::ScaleBlock(unsigned int ID) : Component(ID),
    m_startScale(),
    m_endScale(),
    m_isEnabled(true)
{
    Reset();
}


void hd::animation::ScaleBlock::Reset() {
    m_startScale.Set(1, 1, 1);
    m_endScale.Set(1, 1, 1);
    m_isEnabled = true;
}


void hd::animation::ScaleBlock::Reverse() {
    Vec3 tmp = m_endScale;
    m_endScale = m_startScale;
    m_startScale = tmp;
}


void hd::animation::ScaleBlock::Enable() {
    m_isEnabled = true;
}


void hd::animation::ScaleBlock::Disable() {
    m_isEnabled = false;
}


bool hd::animation::ScaleBlock::IsEnabled() const {
    return m_isEnabled;
}


void hd::animation::ScaleBlock::SetStartScale(const Vec3 &position) {
    m_startScale = position;
}


void hd::animation::ScaleBlock::SetEndScale(const Vec3 &position) {
    m_endScale = position;
}


Vec3 hd::animation::ScaleBlock::GetScale(const float timer) const {
    Vec3 scale;
    if (this->IsEnabled()) {
        scale = hd::lerp(m_startScale, m_endScale, timer);
        return scale;
    } else {
        return Vec3(1, 1, 1);
    }
}
