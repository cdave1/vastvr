#include "MeshVertexBuffer.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/MeshData.h"
#include "graphics/shader/ShaderLoader.h"
#include "graphics/shader/ShaderProgram.h"
#include "graphics/components/Shader.h"
#include "graphics/gl/GraphicsInterface.h"

static uint32_t m_transformBufferSize = 4096;
static uint32_t m_currentVertexArrayObject = UINT32_MAX;
static uint32_t m_depthBufferSize = 4096;

static const char * depthPassVertexShaderSrc = \
"#version 330\n"
"in vec4 iPositionDepth;\n"
"in mat4 iTransformDepth;\n"
"uniform mat4 ProjectionMatrix;\n"
"uniform mat4 ViewMatrix;\n"
"out float depth;\n"
"void main() {\n"
"   vec4 position = (ProjectionMatrix * ViewMatrix * iTransformDepth * iPositionDepth);\n"
"   gl_Position = position;\n"
"   depth = position.z;// / 60000.;\n"
"}\n";

static const char * depthPassFragmentShaderSrc = \
"#version 330\n"
"in float depth;\n"
"void main() {\n"
"   //gl_FragDepth = depth;\n"
"}\n";

static const hd::graphics::ShaderProgram *m_depthPassShader = nullptr;

using namespace hd::graphics;

MeshVertexBuffer::MeshVertexBuffer() :
m_vertexBufferObject(0),
m_vertexArrayObject(0),
m_lineIndexBufferObject(0),
m_triangleIndexBufferObject(0),
m_quadIndexBufferObject(0),
m_transformBufferObject(0),
m_depthVertexBufferObject(0),
m_depthVertexArrayObject(0),
m_depthPassBufferObject(0),
m_primitive(0),
m_vertexCount(0),
m_triangleCount(0),
m_quadCount(0),
m_usage(GL_STATIC_DRAW),
m_indexSize(sizeof(uint32_t)) {
    Reset();
}


void MeshVertexBuffer::Reset() {
    if (m_vertexBufferObject != 0) {
        glDeleteBuffers(1, &m_vertexBufferObject);
        m_vertexBufferObject = 0;
    }

    if (m_lineIndexBufferObject) {
        glDeleteBuffers(1, &m_lineIndexBufferObject);
        m_lineIndexBufferObject = 0;
    }

    if (m_triangleIndexBufferObject) {
        glDeleteBuffers(1, &m_triangleIndexBufferObject);
        m_triangleIndexBufferObject = 0;
    }

    if (m_quadIndexBufferObject) {
        glDeleteBuffers(1, &m_quadIndexBufferObject);
        m_quadIndexBufferObject = 0;
    }

    if (m_transformBufferObject) {
        glDeleteBuffers(1, &m_transformBufferObject);
        m_transformBufferObject = 0;
    }

    if (m_depthPassBufferObject) {
        glDeleteBuffers(1, &m_depthPassBufferObject);
        m_depthPassBufferObject = 0;
    }
}


void MeshVertexBuffer::SetStatic() {
    m_usage = GL_STATIC_DRAW;
}


void MeshVertexBuffer::SetDynamic() {
    m_usage = GL_DYNAMIC_DRAW;
}


void MeshVertexBuffer::SetMesh(const hd::graphics::Mesh *meshData) {
    Reset();
    if (m_vertexArrayObject == 0) glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    const hd::Vector<Vertex> *vertices = meshData->GetVertices();
    m_vertexCount = vertices->Size();
    m_primitive = GL_TRIANGLE_FAN;

    {
        CompressedVertex *buffer = (CompressedVertex *)malloc(sizeof(CompressedVertex) * m_vertexCount);
        for (uint32_t i = 0; i < vertices->Size(); ++i) {
            Vertex vertex = vertices->At(i);
            CompressedVertex compressed(vertex);
            buffer[i] = compressed;
        }

        if (m_vertexBufferObject == 0) {
            glGenBuffers(1, &m_vertexBufferObject);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(CompressedVertex) * m_vertexCount, buffer, m_usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        FreeSafely(buffer);
    }

    { // Depth vertices.
        Vec3 *depthVertexBuffer = (Vec3 *)malloc(sizeof(Vec3) * m_vertexCount);
        for (uint32_t i = 0; i < vertices->Size(); ++i) {
            Vertex vertex = vertices->At(i);
            depthVertexBuffer[i] = vertex.origin;
        }

        if (m_depthVertexBufferObject == 0) {
            glGenBuffers(1, &m_depthVertexBufferObject);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_depthVertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * m_vertexCount, depthVertexBuffer, m_usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        FreeSafely(depthVertexBuffer);
    }

    { // Line Segments
        const hd::Vector<Size2> *lines = meshData->GetLines();
        if (lines->Size() > 0) {
            m_lineCount = lines->Size();

            Size2 *lineIndexBuffer = (Size2 *)malloc(sizeof(Size2) * m_lineCount);
            for (uint32_t i = 0; i < lines->Size(); ++i) {
                Size2 line = lines->At(i);
                lineIndexBuffer[i] = line;
            }

            if (m_lineIndexBufferObject == 0) {
                glGenBuffers(1, &m_lineIndexBufferObject);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lineIndexBufferObject);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Size2) * m_lineCount, lineIndexBuffer, m_usage);
            FreeSafely(lineIndexBuffer);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    { // Triangles
        const hd::Vector<Size3> *triangles = meshData->GetTriangles();
        if (triangles->Size() > 0) {
            m_triangleCount = triangles->Size();

            if (m_triangleCount < USHRT_MAX) {
                m_indexSize = sizeof(uint16_t);

                uint16_t *indexBuffer = (uint16_t *)malloc(sizeof(uint16_t) * 3 * m_triangleCount);
                uint16_t index = 0;
                for (uint32_t i = 0; i < triangles->Size(); ++i) {
                    Size3 triangle = triangles->At(i);
                    indexBuffer[index + 0] = uint16_t(triangle[0]);
                    indexBuffer[index + 1] = uint16_t(triangle[1]);
                    indexBuffer[index + 2] = uint16_t(triangle[2]);
                    index += 3;
                }

                if (m_triangleIndexBufferObject == 0) {
                    glGenBuffers(1, &m_triangleIndexBufferObject);
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triangleIndexBufferObject);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3 * m_triangleCount, indexBuffer, m_usage);
                FreeSafely(indexBuffer);
            } else {
                m_indexSize = sizeof(uint32_t);
                Size3 *indexBuffer = (Size3 *)malloc(sizeof(Size3) * m_triangleCount);
                for (uint32_t i = 0; i < triangles->Size(); ++i) {
                    Size3 triangle = triangles->At(i);
                    indexBuffer[i] = triangle;
                }

                if (m_triangleIndexBufferObject == 0) {
                    glGenBuffers(1, &m_triangleIndexBufferObject);
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triangleIndexBufferObject);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Size3) * m_triangleCount, indexBuffer, m_usage);
                FreeSafely(indexBuffer);
            }
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    { // Quad strip
        const hd::Vector<Size4> *quads = meshData->GetQuads();
        if (quads->Size() > 0) {
            m_quadCount = quads->Size();

            Size4 *indexBuffer = (Size4 *)malloc(sizeof(Size4) * m_quadCount);
            for (uint32_t i = 0; i < quads->Size(); ++i) {
                Size4 quad = quads->At(i);
                indexBuffer[i] = quad;
            }

            if (m_quadIndexBufferObject == 0) {
                glGenBuffers(1, &m_quadIndexBufferObject);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIndexBufferObject);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Size4) * m_quadCount, indexBuffer, m_usage);
            FreeSafely(indexBuffer);
        }
    }

    if (m_depthVertexArrayObject == 0) {
        glGenVertexArrays(1, &m_depthVertexArrayObject);
    }

    if (m_vertexArrayObject == 0) {
        glGenVertexArrays(1, &m_vertexArrayObject);
    }

    m_currentVertexArrayObject = 0;
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Tranform buffer for instancing.
    if (m_transformBufferObject == 0) {
        glGenBuffers(1, &m_transformBufferObject);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_transformBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_transformBufferSize * sizeof(MeshInstanceData), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->SetDepthPassAttributeLocations();
}


void MeshVertexBuffer::SetDefaultAttributeLocations(const Shader *shader) const {
    const char * POSITION_ATTRIBUTE_NAME = "iPosition";
    const char * COLOR_ATTRIBUTE_NAME = "iDiffuseColor";
    const char * NORMAL_ATTRIBUTE_NAME = "iNormal";
    const char * TEXCOORD_ATTRIBUTE_NAME = "iTexCoord";

    int32_t originLocation = shader->GetAttributeLocation(POSITION_ATTRIBUTE_NAME);
    int32_t colorLocation = shader->GetAttributeLocation(COLOR_ATTRIBUTE_NAME);
    int32_t normalLocation = shader->GetAttributeLocation(NORMAL_ATTRIBUTE_NAME);
    int32_t texCoordLocation = shader->GetAttributeLocation(TEXCOORD_ATTRIBUTE_NAME);

    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

    if (m_lineIndexBufferObject > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lineIndexBufferObject);
    }

    if (m_triangleIndexBufferObject > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triangleIndexBufferObject);
    }

    if (m_quadIndexBufferObject > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIndexBufferObject);
    }

    if (originLocation >= 0) {
        glEnableVertexAttribArray(originLocation);
        glVertexAttribPointer(originLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CompressedVertex), BUFFER_OFFSET(0));
    }

    if (colorLocation >= 0) {
        glEnableVertexAttribArray(colorLocation);
        glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(CompressedVertex), BUFFER_OFFSET(sizeof(float) * 3));
    }

    if (normalLocation >= 0) {
        glEnableVertexAttribArray(normalLocation);
        glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CompressedVertex), BUFFER_OFFSET(sizeof(float) * 4));
    }

    if (texCoordLocation >= 0) {
        glEnableVertexAttribArray(texCoordLocation);
        glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(CompressedVertex), BUFFER_OFFSET(sizeof(float) * 7));
    }

    m_currentVertexArrayObject = 0;
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void MeshVertexBuffer::SetDepthPassAttributeLocations() const {
    if (m_depthPassShader == nullptr) {
        m_depthPassShader = hd::graphics::ShaderLoader::Instance()->Get(DEPTHPASS_VERTEX_SHADER,
                                                                        depthPassVertexShaderSrc,
                                                                        DEPTHPASS_FRAGMENT_SHADER,
                                                                        depthPassFragmentShaderSrc);
    }
    
    { // Transform buffer
        glBindVertexArray(m_depthVertexArrayObject);

        if (m_depthPassBufferObject == 0) {
            glGenBuffers(1, &m_depthPassBufferObject);
        }
        glBindBuffer(GL_ARRAY_BUFFER, m_depthPassBufferObject);
        glBufferData(GL_ARRAY_BUFFER, m_depthBufferSize * sizeof(Matrix), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // HACK HACK HACK
        //
        // const_cast to get around ShaderProgram const correctness!!!
        int32_t transformLocation = const_cast<ShaderProgram *>(m_depthPassShader)->GetAttributeLocation("iTransformDepth");
        glBindBuffer(GL_ARRAY_BUFFER, m_depthPassBufferObject);
        if (transformLocation >= 0) {
            for (int i = 0; i < 4; ++i) {
                glEnableVertexAttribArray(transformLocation + i);
                glVertexAttribPointer(transformLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), BUFFER_OFFSET(sizeof(float) * i * 4));
                glVertexAttribDivisor(transformLocation + i, 1);
            }
        } else {
            hd::log::i("");
        }

        m_currentVertexArrayObject = 0;
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    { // Position location
        const char * POSITION_ATTRIBUTE_NAME = "iPositionDepth";
        int32_t originLocation = const_cast<ShaderProgram *>(m_depthPassShader)->GetAttributeLocation(POSITION_ATTRIBUTE_NAME);

        glBindVertexArray(m_depthVertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_depthVertexBufferObject);

        if (m_lineIndexBufferObject > 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lineIndexBufferObject);
        }

        if (m_triangleIndexBufferObject > 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triangleIndexBufferObject);
        }

        if (m_quadIndexBufferObject > 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIndexBufferObject);
        }

        if (originLocation >= 0) {
            glEnableVertexAttribArray(originLocation);
            glVertexAttribPointer(originLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3) , BUFFER_OFFSET(0));
        } else {
            hd::log::i("");
        }

        m_currentVertexArrayObject = 0;
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}



void MeshVertexBuffer::SetInstancedAttributeLocations(const Shader *shader) const {
    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_transformBufferObject);
    int32_t transformLocation = shader->GetAttributeLocation(TRANSFORM_ATTRIBUTE_NAME);
    if (transformLocation >= 0) {
        // The "size" variable of glVertexAttribPointer has a max size of 4 :(
        for (int i = 0; i < 4; ++i) {
            glEnableVertexAttribArray(transformLocation + i);
            glVertexAttribPointer(transformLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(MeshInstanceData), BUFFER_OFFSET(sizeof(float) * i * 4));
            glVertexAttribDivisor(transformLocation + i, 1);
        }
    }

    int32_t ambientColorLocation = shader->GetAttributeLocation(hd::graphics::MATERIAL_AMBIENT_COLOR_UNIFORM);
    if (ambientColorLocation >= 0) {
        glEnableVertexAttribArray(ambientColorLocation);
        glVertexAttribPointer(ambientColorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MeshInstanceData), BUFFER_OFFSET(sizeof(Matrix)));
        glVertexAttribDivisor(ambientColorLocation, 1);
    }

    int32_t diffuseColorLocation = shader->GetAttributeLocation(hd::graphics::MATERIAL_DIFFUSE_COLOR_UNIFORM);
    if (diffuseColorLocation >= 0) {
        glEnableVertexAttribArray(diffuseColorLocation);
        glVertexAttribPointer(diffuseColorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MeshInstanceData), BUFFER_OFFSET(sizeof(Matrix) + sizeof(Color4ub)));
        glVertexAttribDivisor(diffuseColorLocation, 1);
    }

    int32_t specularColorLocation = shader->GetAttributeLocation(hd::graphics::MATERIAL_SPECULAR_COLOR_UNIFORM);
    if (specularColorLocation >= 0) {
        glEnableVertexAttribArray(specularColorLocation);
        glVertexAttribPointer(specularColorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MeshInstanceData), BUFFER_OFFSET(sizeof(Matrix) + 2 * sizeof(Color4ub)));
        glVertexAttribDivisor(specularColorLocation, 1);
    }

    int32_t shininessValueLocation = shader->GetAttributeLocation(hd::graphics::MATERIAL_SHININESS_UNIFORM);
    if (shininessValueLocation >= 0) {
        glEnableVertexAttribArray(shininessValueLocation);
        glVertexAttribPointer(shininessValueLocation, 1, GL_FLOAT, GL_FALSE, sizeof(MeshInstanceData), BUFFER_OFFSET(sizeof(Matrix) + 3 * sizeof(Color4ub)));
        glVertexAttribDivisor(shininessValueLocation, 1);
    }

    m_currentVertexArrayObject = 0;
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void MeshVertexBuffer::RenderLines() const {
    glBindVertexArray(m_vertexArrayObject);
    m_currentVertexArrayObject = m_vertexArrayObject;
    if (m_lineIndexBufferObject > 0) {
        glDrawRangeElements(GL_LINES, 0, m_vertexCount - 1, m_lineCount * 2, GL_UNSIGNED_INT, NULL);
    } else if (m_triangleIndexBufferObject > 0) {
        if (m_indexSize == sizeof(uint16_t)) {
            glDrawElements(GL_LINES, m_triangleCount * 3, GL_UNSIGNED_SHORT, NULL);
        } else {
            glDrawElements(GL_LINES, m_triangleCount * 3, GL_UNSIGNED_INT, NULL);
        }
        //glDrawRangeElements(GL_LINE_LOOP, 0, m_vertexCount - 1, m_lineCount * 2, GL_UNSIGNED_INT, NULL);
    }
    glBindVertexArray(0);
}


void MeshVertexBuffer::RenderDepthInstanced(const Matrix *cameraData, const Matrix *instanceData, const uint32_t count) const {
    if (count == 0 || m_depthPassBufferObject == 0) {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_depthPassBufferObject);
    Matrix *instanceDataBuffer = (Matrix *)glMapBufferRange(GL_ARRAY_BUFFER, 0, count * sizeof(Matrix), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(instanceDataBuffer, instanceData, count * sizeof(Matrix));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

   // if (m_currentVertexArrayObject != m_depthVertexArrayObject) {
        glBindVertexArray(0);
        glBindVertexArray(m_depthVertexArrayObject);
     //   m_currentVertexArrayObject = m_depthVertexArrayObject;
    //}
    m_depthPassShader->Load();
    m_depthPassShader->SetUniformMatrix("ProjectionMatrix", cameraData[0]);
    m_depthPassShader->SetUniformMatrix("ViewMatrix", cameraData[1]);

    if (m_indexSize == sizeof(uint16_t)) {
        glDrawElementsInstanced(GL_TRIANGLES, m_triangleCount * 3, GL_UNSIGNED_SHORT, NULL, count);
    } else {
        glDrawElementsInstanced(GL_TRIANGLES, m_triangleCount * 3, GL_UNSIGNED_INT, NULL, count);
    }

    hd::gl::ResetActiveTexture();
            hd::gl::BindTexture(NULL);
}


void MeshVertexBuffer::RenderInstanced(const MeshInstanceData *instanceData, const uint32_t count) const {
    if (count == 0 || m_transformBufferObject == 0) {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_transformBufferObject);
    MeshInstanceData *instanceDataBuffer = (MeshInstanceData *)glMapBufferRange(GL_ARRAY_BUFFER, 0, count * sizeof(MeshInstanceData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(instanceDataBuffer, instanceData, count * sizeof(MeshInstanceData));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VertexBuffer and the depth pass also change VAO bindings. Bind this
    // mesh explicitly rather than relying on a cache shared by mesh draws.
    glBindVertexArray(m_vertexArrayObject);
    m_currentVertexArrayObject = m_vertexArrayObject;

    if (m_indexSize == sizeof(uint16_t)) {
        glDrawElementsInstanced(GL_TRIANGLES, m_triangleCount * 3, GL_UNSIGNED_SHORT, NULL, count);
    } else {
        glDrawElementsInstanced(GL_TRIANGLES, m_triangleCount * 3, GL_UNSIGNED_INT, NULL, count);
    }
}


void MeshVertexBuffer::Render() const {
    // VertexBuffer and the depth pass also change VAO bindings. Bind this
    // mesh explicitly rather than relying on a cache shared by mesh draws.
    glBindVertexArray(m_vertexArrayObject);
    m_currentVertexArrayObject = m_vertexArrayObject;
    if (m_triangleIndexBufferObject > 0) {
        if (m_indexSize == sizeof(uint16_t)) {
            glDrawElements(GL_TRIANGLES, m_triangleCount * 3, GL_UNSIGNED_SHORT, NULL);
        } else {
            glDrawElements(GL_TRIANGLES, m_triangleCount * 3, GL_UNSIGNED_INT, NULL);
        }
    }
}
