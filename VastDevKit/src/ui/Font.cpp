#include "Font.h"
#include "graphics/components/Shader.h"
#include <difont/difont.h>

hd::ui::Font::Font(unsigned int ID) : Component(ID),
    m_faceSize(10.0f),
    m_fontName(),
    m_font(NULL) {}


void hd::ui::Font::SetFont(difont::Font *font) {
    if (font->Error()) {
        FT_Error error = font->Error();
        hd::log::i("Error with font: ");
    }
    m_font = font;
}


void hd::ui::Font::SetFaceSize(const float faceSize) {
    m_faceSize = faceSize;
    if (m_font) {
        m_font->FaceSize(uint32_t(faceSize));
        // Resizing preserves the character map selected by FontLoader.
    }
}


AABB hd::ui::Font::GetSize(const std::string &text) {
    if (m_font) {
        difont::BBox ftbox = m_font->BBox(text.c_str());
        Vec3 lower(ftbox.Lower().Xf(), max(ftbox.Lower().Yf(), 0.0f), ftbox.Lower().Zf());
        Vec3 upper(ftbox.Upper().X(), ftbox.Upper().Y(), ftbox.Upper().Z());
        AABB ftboxAABB(lower, upper);
        
        m_bearingX = ftbox.Lower().Xf();

        Vec3 lo;
        Vec3 hi(lo[0] + (ftboxAABB.Width()),
                lo[1] + (ftboxAABB.Height()),
                lo[2] + (ftboxAABB.Depth()));
        return AABB(lo, hi);
    } else {
        return AABB();
    }
}


difont::Font *hd::ui::Font::GetFont() const {
    return m_font;
}
