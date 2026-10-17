#include "FontRenderer.h"
#include "ui/Font.h"
#include "ui/FontLayout.h"
#include "graphics/components/Shader.h"
#include <difont/difont.h>

hd::ui::FontRenderer::FontRenderer(unsigned int ID) : Component(ID),
    m_originLocation(0),
    m_colorLocation(0),
    m_normalLocation(0),
    m_tangentLocation(0),
    m_texCoordLocation(0) {}


void hd::ui::FontRenderer::SetDefaultAttributeLocations(const hd::graphics::Shader *shader) {
    const char * POSITION_ATTRIBUTE_NAME = "iPosition";
    const char * NORMAL_ATTRIBUTE_NAME = "iNormal";
    const char * COLOR_ATTRIBUTE_NAME = "iColor";
    const char * TANGENT_ATTRIBUTE_NAME = "iTangent";
    const char * TEXCOORD_ATTRIBUTE_NAME = "iTexCoord";

    int32_t originLocation = shader->GetAttributeLocation(POSITION_ATTRIBUTE_NAME);
    int32_t normalLocation = shader->GetAttributeLocation(NORMAL_ATTRIBUTE_NAME);
    int32_t colorLocation = shader->GetAttributeLocation(COLOR_ATTRIBUTE_NAME);
    int32_t tangentLocation = shader->GetAttributeLocation(TANGENT_ATTRIBUTE_NAME);
    int32_t texCoordLocation = shader->GetAttributeLocation(TEXCOORD_ATTRIBUTE_NAME);

    this->SetOriginAttributeLocation(originLocation);
    this->SetNormalAttributeLocation(normalLocation);
    this->SetColorAttributeLocation(colorLocation);
    this->SetTangentAttributeLocation(tangentLocation);
    this->SetTexCoordAttributeLocation(texCoordLocation);
}


void hd::ui::FontRenderer::SetOriginAttributeLocation(const int32_t location) {
    m_originLocation = location;
}


void hd::ui::FontRenderer::SetColorAttributeLocation(const int32_t location) {
    m_colorLocation = location;
}


void hd::ui::FontRenderer::SetNormalAttributeLocation(const int32_t location) {
    m_normalLocation = location;
}


void hd::ui::FontRenderer::SetTangentAttributeLocation(const int32_t location) {
    m_tangentLocation = location;
}


void hd::ui::FontRenderer::SetTexCoordAttributeLocation(const int32_t location) {
    m_texCoordLocation = location;
}


void hd::ui::FontRenderer::Render(hd::ui::Font *font, std::string text) const {
    if (!font) {
        hd::log::e("Can't render a null font.\n");
        return;
    }

    difont::Font *difontFont = font->GetFont();

    if (difontFont) {
        difontFont->Render(text.c_str());
    }
}


void hd::ui::FontRenderer::Render(hd::ui::Font *font, hd::ui::FontLayout *layout, std::string text) const {
    if (!font || !layout) {
        hd::log::e("Can't render a null font and layout.\n");
        return;
    }

    layout->Update();
    difont::Font *difontFont = font->GetFont();
    difont::SimpleLayout *difontLayout = const_cast<difont::SimpleLayout *>(layout->GetLayout());

    if (difontFont && difontLayout) {
        difontLayout->SetFont(difontFont);
        difontLayout->Render(text.c_str(), -1, difont::Point(), difont::RENDER_FRONT);
    }
}
