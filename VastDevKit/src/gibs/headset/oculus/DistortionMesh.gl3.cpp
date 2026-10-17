#include "DistortionMesh.h"
#include "graphics/components/Shader.h"
#include "graphics/gl/GraphicsInterface.h"
#include "gibs/headset/oculus/DistortionVertex.h"

using namespace hd::graphics;


hd::input::DistortionMesh::DistortionMesh() :
    m_vertexArrayObject(0),
    m_vertexBufferObject(0),
    m_vertexCount(0),
    m_indexBufferObject(0),
    m_indexCount(0) {}


void hd::input::DistortionMesh::Reset() {
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
    for (uint32_t i = 0; i < vertices->Size(); ++i) {
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
    const char * POSITION_ATTRIBUTE_NAME = "iPosition";
    const char * TIMEWARP_FACTOR_NAME = "iTimewarpLerpFactor";
    const char * VIGNETTE_NAME = "iVignette";
    const char * TEX_COORD_0_NAME = "iTexCoord0";
    const char * TEX_COORD_1_NAME = "iTexCoord1";
    const char * TEX_COORD_2_NAME = "iTexCoord2";

    int32_t positionLocation = shader->GetAttributeLocation(POSITION_ATTRIBUTE_NAME);
    int32_t timewarpLocation = shader->GetAttributeLocation(TIMEWARP_FACTOR_NAME);
    int32_t vignetteLocation = shader->GetAttributeLocation(VIGNETTE_NAME);
    int32_t texCoord0Location = shader->GetAttributeLocation(TEX_COORD_0_NAME);
    int32_t texCoord1Location = shader->GetAttributeLocation(TEX_COORD_1_NAME);
    int32_t texCoord2Location = shader->GetAttributeLocation(TEX_COORD_2_NAME);

    if (m_vertexArrayObject == 0) {
        glGenVertexArrays(1, &m_vertexArrayObject);
    }

        glBindVertexArray(m_vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);

    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL));

    glEnableVertexAttribArray(timewarpLocation);
    glVertexAttribPointer(timewarpLocation, 1, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 2)));

    glEnableVertexAttribArray(vignetteLocation);
    glVertexAttribPointer(vignetteLocation, 1, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 3)));

    glEnableVertexAttribArray(texCoord0Location);
    glVertexAttribPointer(texCoord0Location, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 4)));

    glEnableVertexAttribArray(texCoord1Location);
    glVertexAttribPointer(texCoord1Location, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 6)));

    glEnableVertexAttribArray(texCoord2Location);
    glVertexAttribPointer(texCoord2Location, 2, GL_FLOAT, GL_FALSE, sizeof(DistortionVertex), ((char *)NULL + (sizeof(float) * 8)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void hd::input::DistortionMesh::Render() {
    glBindVertexArray(m_vertexArrayObject);
    glDrawRangeElements(GL_TRIANGLES, 0, m_vertexCount - 1, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}
