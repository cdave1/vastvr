#include "FontLayout.h"
#include "graphics/components/Shader.h"
#include "ui/fonts/FontLoader.h"


difont::TextAlignment ConvertTextAlignment(hd::ui::TextAlignment alignment) {
    if (alignment == hd::ui::TextAlignment::LEFT) {
        return difont::ALIGN_LEFT;
    } else if (alignment == hd::ui::TextAlignment::CENTERED) {
        return difont::ALIGN_CENTER;
    } else if (alignment == hd::ui::TextAlignment::JUSTIFY) {
        return difont::ALIGN_JUSTIFY;
    } else if (alignment == hd::ui::TextAlignment::RIGHT) {
        return difont::ALIGN_RIGHT;
    } else {
        return difont::ALIGN_LEFT;
    }
}


hd::ui::FontLayout::FontLayout(unsigned int ID) : Component(ID),
    m_alignment(hd::ui::TextAlignment::LEFT),
    m_lineLength(250.0f),
    m_lineSpacing(0.75f),
    m_layout() {
        Reset();
}


void hd::ui::FontLayout::Reset() {
    m_layout.SetLineLength(m_lineLength);
    m_layout.SetLineSpacing(m_lineSpacing);
    m_layout.SetAlignment(ConvertTextAlignment(m_alignment));
    m_layout.SetFont(hd::ui::FontLoader::Instance()->FindTextureFont("DEFAULT", 24));
}


void hd::ui::FontLayout::Update() {
    m_layout.SetLineLength(m_lineLength);
    m_layout.SetLineSpacing(m_lineSpacing);
    m_layout.SetAlignment(ConvertTextAlignment(m_alignment));
}


void hd::ui::FontLayout::SetAlignment(hd::ui::TextAlignment alignment) {
    m_alignment = alignment;
}


void hd::ui::FontLayout::SetLineLength(float lineLength) {
    m_lineLength = lineLength;
}


void hd::ui::FontLayout::SetLineSpacing(float lineSpacing) {
    m_lineSpacing = lineSpacing;
}


AABB hd::ui::FontLayout::GetSize(difont::Font *font, const std::string &text) {
    m_layout.SetFont(font);
    difont::BBox ftbox = m_layout.BBox(text.c_str());
    AABB ftboxAABB(Vec3(ftbox.Lower().X(), ftbox.Lower().Y(), ftbox.Lower().Z()),
                   Vec3(ftbox.Upper().X(), ftbox.Upper().Y(), ftbox.Upper().Z()));
    return ftboxAABB;
}


const difont::SimpleLayout * hd::ui::FontLayout::GetLayout() const {
    return &m_layout;
}
