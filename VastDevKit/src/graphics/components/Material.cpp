#include "Material.h"
#include "graphics/Texture/Texture.h"
#include "graphics/Texture/TextureManager.h"
#include "graphics/gl/GraphicsInterface.h"

using namespace hd;
using namespace hd::graphics;

Material::Material(unsigned int ID) : Component(ID),
    m_colorTexture(nullptr),
    m_normalTexture(nullptr),
    m_alphaMaskTexture(nullptr),
    m_bumpTexture(nullptr),
    m_ambient(1.0f, 1.0f, 1.0f, 1.0f),
    m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
    m_specular(0.0f, 0.0f, 0.0f, 1.0f),
    m_emission(0.0f, 0.0f, 0.0f, 1.0f),
    m_ambientAdjustment(0.0f, 0.0f, 0.0f, 0.0f),
    m_shininess(1.0f) {}


void Material::Reset() {
    m_ambient = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
    m_diffuse = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
    m_specular = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_emission = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_ambientAdjustment = Color4f(0.0f, 0.0f, 0.0f, 0.0f);
    m_shininess = 1.0f;
    
    m_colorTexture = nullptr;
}


void hd::graphics::Material::SetTexture(Texture *texture) {
    m_colorTexture = texture;
}


void hd::graphics::Material::SetColorTexture(Texture *texture) {
    m_colorTexture = texture;
}


void hd::graphics::Material::SetNormalTexture(Texture *texture) {
    m_normalTexture = texture;
}


void hd::graphics::Material::SetAlphaMaskTexture(Texture *texture) {
    m_alphaMaskTexture = texture;
}


void hd::graphics::Material::SetBumpTexture(Texture *texture) {
    m_bumpTexture = texture;
}


const Texture * hd::graphics::Material::GetTexture() const {
    return m_colorTexture;
}


hd::Texture * hd::graphics::Material::GetColorTexture() const {
    return  m_colorTexture;
}


hd::Texture * hd::graphics::Material::GetNormalTexture() const {
    return m_normalTexture;
}


hd::Texture * hd::graphics::Material::GetAlphaMaskTexture() const {
    return m_alphaMaskTexture;
}


hd::Texture * hd::graphics::Material::GetBumpTexture() const {
    return m_bumpTexture;
}


void Material::SetColor(const Color4f &color) {
    SetAmbient(color);
}


const Color4f & Material::GetAmbient() const {
    return m_ambient;
}


void Material::SetAmbient(const Color4f &ambient) {
    m_ambient = ambient;
}


const Color4f & Material::GetDiffuse() const {
    return m_diffuse;
}


void Material::SetDiffuse(const Color4f &diffuse) {
    m_diffuse = diffuse;
}


const Color4f & Material::GetSpecular() const {
    return m_specular;
}


void Material::SetSpecular(const Color4f &specular) {
    m_specular = specular;
}


const Color4f & Material::GetEmission() const {
    return m_emission;
}


void Material::SetEmission(const Color4f &emission) {
    m_emission = emission;
}


const Color4f & Material::GetAmbientAdjustment() const {
    return m_ambientAdjustment;
}


void Material::SetAmbientAdjustment(const Color4f &ambient) {
    m_ambientAdjustment = ambient;
}


float Material::GetShininess() const {
    return m_shininess;
}


void Material::SetShininess(const float shininess) {
    m_shininess = shininess;
}


void Material::Render() const {
    Color4f ambient = Color4f(hd::clamp(m_ambient[0] + m_ambientAdjustment[0], 0.0f, 1.0f),
                              hd::clamp(m_ambient[1] + m_ambientAdjustment[1], 0.0f, 1.0f),
                              hd::clamp(m_ambient[2] + m_ambientAdjustment[2], 0.0f, 1.0f),
                              hd::clamp(m_ambient[3] + m_ambientAdjustment[3], 0.0f, 1.0f));
/*    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse.f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular.f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission.f);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);*/
}
