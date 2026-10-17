#include "OpacityData.h"

hd::graphics::OpacityData::OpacityData(unsigned int ID) : Component(ID),
    m_opacity(1.0f),
    m_opacityRange(1.0f) {
    Reset();
}


void hd::graphics::OpacityData::Reset() {
    m_opacity = 1.0f;
}


float hd::graphics::OpacityData::GetOpacity() const {
    if (m_opacityRange == 0.0f) {
        return 0.0f;
    } else {
        float op = m_opacity / m_opacityRange;
        return hd::clamp(op, 0.0f, m_opacityRange);
    }
}


float hd::graphics::OpacityData::GetRealOpacity() const {
    return m_opacity;
}


void hd::graphics::OpacityData::SetOpacityRange(const float opacityRange) {
    m_opacityRange = hd::clamp(opacityRange, 0.0f, 1.0f);
}


void hd::graphics::OpacityData::SetOpacity(const float opacity) {
    m_opacity = hd::clamp(opacity, 0.0f, 1.0f);
}


bool hd::graphics::OpacityData::IsVisible() const {
    return (GetOpacity() > 0.0f);
}
