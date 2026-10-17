#include "Light.h"

#include "graphics/gl/GraphicsInterface.h"

using namespace hd;
using namespace hd::graphics;

Light::Light(unsigned int ID) : Component(ID),
    m_position(),
    m_spotDirection(),
    m_spotExponent(0),
    m_spotCutoff(0),
    m_constantAttenuation(0),
    m_linearAttenuation(0),
    m_quadraticAttenuation(0),
    m_isDirectional(false),
    m_ambient(0, 0, 0, 1),
    m_diffuse(0, 0, 0, 0),
    m_specular(0, 0, 0, 0)
{}


void Light::Reset() {
    m_position = Vec3();
    m_spotDirection = Vec3();
    m_spotExponent = 0.0f;
    m_spotCutoff = 0.0f;
    m_constantAttenuation = 0.0f;
    m_linearAttenuation = 0.0f;
    m_quadraticAttenuation = 0.0f;
    m_isDirectional = false;
    m_ambient = Color4f(0, 0, 0, 1);
    m_diffuse = Color4f(0, 0, 0, 0);
    m_specular = Color4f(0, 0, 0, 0);
}


//
// Getters
//
const Vec3 & Light::GetPosition() const {
    return m_position;
}


const Vec3 & Light::GetSpotDirection() const {
    return m_spotDirection;
}


float Light::GetSpotExponent() const {
    return m_spotExponent;
}


float Light::GetSpotCutoff() const {
    return m_spotCutoff;
}


float Light::GetConstantAttenuation() const {
    return m_constantAttenuation;
}


float Light::GetLinearAttenuation() const {
    return m_linearAttenuation;
}


float Light::GetQuadraticAttenuation() const {
    return m_quadraticAttenuation;
}


bool Light::IsDirectional() const {
    return m_isDirectional;
}


const Color4f & Light::GetAmbient() const {
    return m_ambient;
}


const Color4f & Light::GetDiffuse() const {
    return m_diffuse;
}


const Color4f & Light::GetSpecular() const {
    return m_specular;
}


//
// Setters
//
void Light::SetPosition(const Vec3 &position) {
    m_position = position;
}


void Light::SetSpotDirection(const Vec3 &direction) {
    m_spotDirection = direction;
}


void Light::SetSpotExponent(const float exponent) {
    m_spotExponent = exponent;
}


void Light::SetSpotCutoff(const float cutoff) {
    m_spotCutoff = cutoff;
}


void Light::SetConstantAttenuation(const float constantAttenuation) {
    m_constantAttenuation = constantAttenuation;
}


void Light::SetLinearAttenuation(const float linearAttenuation) {
    m_linearAttenuation = linearAttenuation;
}


void Light::SetQuadraticAttenuation(const float quadraticAttenuation) {
    m_quadraticAttenuation = quadraticAttenuation;
}


void Light::SetIsDirectional(bool isDirectional) {
    m_isDirectional = isDirectional;
}


void Light::SetAmbient(const Color4f &ambient) {
    m_ambient = ambient;
}


void Light::SetDiffuse(const Color4f &diffuse) {
    m_diffuse = diffuse;
}


void Light::SetSpecular(const Color4f &specular) {
    m_specular = specular;
}
