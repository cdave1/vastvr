#include "DistortionMesh.h"
#include "graphics/components/Shader.h"
#include "graphics/gl/GraphicsInterface.h"

using namespace hd::graphics;


hd::input::DistortionMesh::DistortionMesh() :
    m_positionLocation(0),
    m_timewarpLocation(0),
    m_vignetteLocation(0),
    m_texCoord0Location(0),
    m_texCoord1Location(0),
    m_texCoord2Location(0),
    m_vertexBufferObject(0),
    m_vertexCount(0),
    m_indexBufferObject(0),
    m_indexCount(0) {}


void hd::input::DistortionMesh::Reset() {
    m_positionLocation = 0;
    m_timewarpLocation = 0;
    m_vignetteLocation = 0;
    m_texCoord0Location = 0;
    m_texCoord1Location = 0;
    m_texCoord2Location = 0;
    m_indexCount = 0;
    m_vertexCount = 0;

    if (m_vertexBufferObject != 0) {
        glDeleteBuffers(1, &m_vertexBufferObject);
        m_vertexBufferObject = 0;
    }

    if (m_indexBufferObject != 0) {
        glDeleteBuffers(1, &m_indexBufferObject);
        m_indexBufferObject = 0;
    }
}


void hd::input::DistortionMesh::SetVertices(const hd::Vector<hd::input::DistortionVertex> *vertices) {
    m_vertexCount = vertices->Size();

    hd::input::DistortionVertex *buffer = (hd::input::DistortionVertex *)malloc(sizeof(hd::input::DistortionVertex) * vertices->Size());
    for (int i = 0; i < vertices->Size(); ++i) {
        hd::input::DistortionVertex vertex = vertices->At(i);
        buffer[i] = vertex;
    }

    if (m_vertexBufferObject == 0) {
        glGenBuffers(1, &m_vertexBufferObject);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hd::input::DistortionVertex) * vertices->Size(), buffer, GL_STATIC_DRAW);
    hdFreeSafely(buffer);
}


void hd::input::DistortionMesh::SetIndices(unsigned short *data, unsigned int indexCount) {
    if (m_indexBufferObject == 0) {
        glGenBuffers(1, &m_indexBufferObject);
    }

    m_indexCount = indexCount;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indexCount, data, GL_STATIC_DRAW);
}


void hd::input::DistortionMesh::SetDefaultAttributeLocations(const Shader *shader) {
    const std::string POSITION_ATTRIBUTE_NAME = "iPosition";
    const std::string TIMEWARP_FACTOR_NAME = "iTimewarpLerpFactor";
    const std::string VIGNETTE_NAME = "iVignette";
    const std::string TEX_COORD_0_NAME = "iTexCoord0";
    const std::string TEX_COORD_1_NAME = "iTexCoord1";
    const std::string TEX_COORD_2_NAME = "iTexCoord2";

    m_positionLocation = shader->GetAttributeLocation(POSITION_ATTRIBUTE_NAME);
    m_timewarpLocation = shader->GetAttributeLocation(TIMEWARP_FACTOR_NAME);
    m_vignetteLocation = shader->GetAttributeLocation(VIGNETTE_NAME);
    m_texCoord0Location = shader->GetAttributeLocation(TEX_COORD_0_NAME);
    m_texCoord1Location = shader->GetAttributeLocation(TEX_COORD_1_NAME);
    m_texCoord2Location = shader->GetAttributeLocation(TEX_COORD_2_NAME);
}


void hd::input::DistortionMesh::Render() {
    hdAssert(m_positionLocation >= 0);
    hdAssert(m_timewarpLocation >= 0);
    hdAssert(m_vignetteLocation >= 0);
    hdAssert(m_texCoord0Location >= 0);
    hdAssert(m_texCoord1Location >= 0);
    hdAssert(m_texCoord2Location >= 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

    glEnableVertexAttribArray(m_positionLocation);
    glVertexAttribPointer(m_positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL));

    glEnableVertexAttribArray(m_timewarpLocation);
    glVertexAttribPointer(m_timewarpLocation, 1, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 2)));

    glEnableVertexAttribArray(m_vignetteLocation);
    glVertexAttribPointer(m_vignetteLocation, 1, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 3)));

    glEnableVertexAttribArray(m_texCoord0Location);
    glVertexAttribPointer(m_texCoord0Location, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 4)));

    glEnableVertexAttribArray(m_texCoord1Location);
    glVertexAttribPointer(m_texCoord1Location, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 6)));

    glEnableVertexAttribArray(m_texCoord2Location);
    glVertexAttribPointer(m_texCoord2Location, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 8)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, ((char *)NULL));

    glDisableVertexAttribArray(m_positionLocation);
    glDisableVertexAttribArray(m_timewarpLocation);
    glDisableVertexAttribArray(m_vignetteLocation);
    glDisableVertexAttribArray(m_texCoord0Location);
    glDisableVertexAttribArray(m_texCoord1Location);
    glDisableVertexAttribArray(m_texCoord2Location);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
