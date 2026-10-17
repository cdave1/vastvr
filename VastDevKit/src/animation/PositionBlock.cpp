#include "PositionBlock.h"
#include "math/Geometry.h"
#include "graphics/gl/GraphicsInterface.h"

hd::animation::PositionBlock::PositionBlock(unsigned int ID) : Component(ID),
    m_startPosition(),
    m_endPostion(),
    m_isEnabled(true)
{
    Reset();
}


void hd::animation::PositionBlock::Reset() {
    m_startPosition.Set(0, 0, 0);
    m_endPostion.Set(0, 0, 0);
    m_isEnabled = true;
}


void hd::animation::PositionBlock::Reverse() {
    Vec3 tmp = m_endPostion;
    m_endPostion = m_startPosition;
    m_startPosition = tmp;
}


void hd::animation::PositionBlock::Enable() {
    m_isEnabled = true;
}


void hd::animation::PositionBlock::Disable() {
    m_isEnabled = false;
}


bool hd::animation::PositionBlock::IsEnabled() const {
    return m_isEnabled;
}


void hd::animation::PositionBlock::SetStartPosition(const Vec3 &position) {
    m_startPosition = position;
}


void hd::animation::PositionBlock::SetEndPosition(const Vec3 &position) {
    m_endPostion = position;
}


Vec3 hd::animation::PositionBlock::GetPosition(const float timer) const {
    return hd::lerp(m_startPosition, m_endPostion, timer);
}
