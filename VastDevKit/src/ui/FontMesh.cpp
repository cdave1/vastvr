#include "FontMesh.h"
#include "ui/Font.h"
#include "ui/FontLayout.h"
#include "graphics/components/Shader.h"

hd::ui::FontMesh::FontMesh(unsigned int ID) : Component(ID),
    m_currentMesh(NULL),
    m_currentText() {}


hd::graphics::MeshData * hd::ui::FontMesh::GetMeshData() const {
    return m_currentMesh;
}


std::string hd::ui::FontMesh::GetText() const {
    return m_currentText;
}


void hd::ui::FontMesh::RenderMesh(const hd::ui::Font *font, const std::string text) {
    if (!font) {
        hd::log::e("Can't render a null font.\n");
        return;
    }

    if (m_currentText == text) {
        return;
    }

    if (m_currentMesh) {
        hd::graphics::MeshLoader::Instance()->DestroyMeshData(m_currentMesh);
        m_currentMesh = NULL;
    }

    m_currentText = text;

    hd::graphics::MeshData *meshData = hd::graphics::MeshLoader::Instance()->GetFontMesh(font, text);
    m_currentMesh = meshData;
}


void hd::ui::FontMesh::RenderMesh(const hd::ui::Font *font, const hd::ui::FontLayout *layout, std::string text) {
    if (!font && !layout) {
        hd::log::e("Can't render a null font and layout.\n");
        return;
    }

    if (m_currentText == text) {
        return;
    }

    if (m_currentMesh) {
        hd::graphics::MeshLoader::Instance()->DestroyMeshData(m_currentMesh);
        m_currentMesh = NULL;
    }

    m_currentText = text;

    hd::graphics::MeshData *meshData = hd::graphics::MeshLoader::Instance()->GetFontMesh(font, layout, text);
    m_currentMesh = meshData;
}
