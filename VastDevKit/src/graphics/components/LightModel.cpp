#include "LightModel.h"
#include "graphics/components/Curve.h"

using namespace hd::graphics;

LightModel::LightModel(unsigned int ID) : Component(ID),
    m_ambient(1.0f, 1.0f, 1.0f, 1.0f) {}


void LightModel::Reset() {
    m_ambient = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
}


const Color4f & LightModel::GetAmbient() const {
    return m_ambient;
}


void LightModel::SetAmbient(const Color4f &ambient) {
    m_ambient = ambient;
}