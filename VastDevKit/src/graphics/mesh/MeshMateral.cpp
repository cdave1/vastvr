#include "MeshMaterial.h"
#include "graphics/Texture/Texture.h"
#include "graphics/Texture/TextureManager.h"

using namespace hd;
using namespace hd::graphics;

hd::graphics::MeshMaterial::MeshMaterial() :
    m_name("name"),
    m_ambient(1.0f, 1.0f, 1.0f, 1.0f),
    m_diffuse(0.8f, 0.8f, 0.8f, 1.0f),
    m_specular(0.0f, 0.0f, 0.0f, 1.0f),
    m_emission(0.0f, 0.0f, 0.0f, 1.0f),
    m_shininess(0),
    m_colorTexture(NULL),
    m_normalTexture(NULL),
    m_alphaMaskTexture(NULL),
    m_bumpTexture(NULL) {}


hd::graphics::MeshMaterial::~MeshMaterial() {
}


const std::string & hd::graphics::MeshMaterial::GetName() const {
    return m_name;
}


hd::Texture * hd::graphics::MeshMaterial::GetColorTexture() const {
    return  m_colorTexture;
}


hd::Texture * hd::graphics::MeshMaterial::GetNormalTexture() const {
    return m_normalTexture;
}


hd::Texture * hd::graphics::MeshMaterial::GetAlphaMaskTexture() const {
    return m_alphaMaskTexture;
}


hd::Texture * hd::graphics::MeshMaterial::GetBumpTexture() const {
    return m_bumpTexture;
}


const Color4f & hd::graphics::MeshMaterial::GetAmbient() const {
    return m_ambient;
}


const Color4f & hd::graphics::MeshMaterial::GetDiffuse() const {
    return m_diffuse;
}


float hd::graphics::MeshMaterial::GetShininess() const {
    return m_shininess;
}


const Color4f & hd::graphics::MeshMaterial::GetEmission() const {
    return m_emission;
}


const Color4f & hd::graphics::MeshMaterial::GetSpecular() const {
    return m_specular;
}


void hd::graphics::MeshMaterial::SetName(const std::string &name) {
    m_name = name;
}


void hd::graphics::MeshMaterial::SetColorTexture(Texture *texture) {
    m_colorTexture = texture;
}


void hd::graphics::MeshMaterial::SetNormalTexture(Texture *texture) {
    m_normalTexture = texture;
}


void hd::graphics::MeshMaterial::SetAlphaMaskTexture(Texture *texture) {
    m_alphaMaskTexture = texture;
}


void hd::graphics::MeshMaterial::SetBumpTexture(Texture *texture) {
    m_bumpTexture = texture;
}


void hd::graphics::MeshMaterial::SetColor(const Color4f &color) {
    SetAmbient(color);
}


void hd::graphics::MeshMaterial::SetAmbient(const Color4f &ambient) {
    m_ambient = ambient;
}


void hd::graphics::MeshMaterial::SetDiffuse(const Color4f &diffuse) {
    m_diffuse = diffuse;
}


void hd::graphics::MeshMaterial::SetSpecular(const Color4f &specular) {
    m_specular = specular;
}


void hd::graphics::MeshMaterial::SetEmission(const Color4f &emission) {
    m_emission = emission;
}


void hd::graphics::MeshMaterial::SetShininess(const float shininess) {
    m_shininess = shininess;
}
