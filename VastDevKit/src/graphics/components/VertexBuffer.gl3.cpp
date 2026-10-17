#include "VertexBuffer.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/MeshData.h"
#include "graphics/components/Polygon.h"
#include "graphics/components/Shader.h"
#include "graphics/gl/GraphicsInterface.h"

using namespace hd::graphics;


VertexBuffer::VertexBuffer(unsigned int ID) : Component(ID),
m_usage(GL_STATIC_DRAW),
m_buffers(NULL) {
    m_buffers = new hd::Vector<hd::graphics::VertexBufferInfo>(8);
    Reset();
}


void VertexBuffer::Reset() {
    for (uint32_t i = 0; i < m_buffers->Size(); ++i) {
        hd::graphics::VertexBufferInfo bufferData = m_buffers->At(i);

        if (bufferData.vertexArrayObject != 0) {
            glDeleteVertexArrays(1, &bufferData.vertexArrayObject);
        }
        if (bufferData.vertexBufferObject != 0) {
            glDeleteBuffers(1, &bufferData.vertexBufferObject);
            bufferData.vertexBufferObject = 0;
        }

        if (bufferData.triangleIndexBufferObject) {
            glDeleteBuffers(1, &bufferData.triangleIndexBufferObject);
            bufferData.triangleIndexBufferObject = 0;
        }

        if (bufferData.quadIndexBufferObject) {
            glDeleteBuffers(1, &bufferData.quadIndexBufferObject);
            bufferData.quadIndexBufferObject = 0;
        }
    }
    m_buffers->Clear();
}


void VertexBuffer::SetStatic() {
    m_usage = GL_STATIC_DRAW;
}


void VertexBuffer::SetDynamic() {
    m_usage = GL_DYNAMIC_DRAW;
}


void VertexBuffer::AddMesh(const hd::graphics::Mesh *meshData) {
    //
    // HACK HACK HACK
    //
    // Null component continually receives new buffers when this function is called.
    // We check if the component is a null component in this case.  Need a better way of
    // preventing this!!!
    //
    if (IsNullComponent()) {
        return;
    }

    const hd::Vector<Vertex> *vertices = meshData->GetVertices();
    hd::graphics::VertexBufferInfo bufferData;
    // Element-buffer bindings belong to a VAO. Do not overwrite a mesh/depth
    // VAO that a different renderer may have left bound before this upload.
    glGenVertexArrays(1, &bufferData.vertexArrayObject);
    glBindVertexArray(bufferData.vertexArrayObject);
    bufferData.vertexCount = vertices->Size();
    bufferData.primitive = GL_TRIANGLE_FAN;

    Vertex *buffer = (Vertex *)malloc(sizeof(Vertex) * bufferData.vertexCount);
	for (uint32_t i = 0; i < vertices->Size(); ++i) {
        Vertex vertex = vertices->At(i);
        buffer[i] = vertex;
    }

    if (bufferData.vertexBufferObject == 0) {
        glGenBuffers(1, &bufferData.vertexBufferObject);
    }

    glBindBuffer(GL_ARRAY_BUFFER, bufferData.vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * bufferData.vertexCount, buffer, m_usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    FreeSafely(buffer);

    const hd::Vector<Size3> *triangles = meshData->GetTriangles();
    const hd::Vector<Size4> *quads = meshData->GetQuads();

    if (triangles->Size() > 0) {
        bufferData.triangleCount = triangles->Size();

        Size3 *indexBuffer = (Size3 *)malloc(sizeof(Size3) * bufferData.triangleCount);
		for (uint32_t i = 0; i < triangles->Size(); ++i) {
            Size3 triangle = triangles->At(i);
            indexBuffer[i] = triangle;
        }

        if (bufferData.triangleIndexBufferObject == 0) {
            glGenBuffers(1, &bufferData.triangleIndexBufferObject);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData.triangleIndexBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Size3) * bufferData.triangleCount, indexBuffer, m_usage);
        FreeSafely(indexBuffer);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (quads->Size() > 0) {
        bufferData.quadCount = quads->Size();

        Size4 *indexBuffer = (Size4 *)malloc(sizeof(Size4) * bufferData.quadCount);
		for (uint32_t i = 0; i < quads->Size(); ++i) {
            Size4 quad = quads->At(i);
            indexBuffer[i] = quad;
        }

        if (bufferData.quadIndexBufferObject == 0) {
            glGenBuffers(1, &bufferData.quadIndexBufferObject);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData.quadIndexBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Size4) * bufferData.quadCount, indexBuffer, m_usage);
        FreeSafely(indexBuffer);
    }

    if (bufferData.vertexArrayObject == 0) {
        glGenVertexArrays(1, &bufferData.vertexArrayObject);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_buffers->Add(bufferData);
}


void VertexBuffer::SetFromMesh(const hd::graphics::Mesh *meshData) {
    Reset();
    AddMesh(meshData);
}


void VertexBuffer::AddMeshes(const hd::graphics::MeshData *meshes) {
    if (meshes == NULL) {
        hd::log::w("Mesh data was null\n");
        return;
    }

    Reset();
    hd::Vector<hd::graphics::Mesh *> *meshList = meshes->GetMeshes();
	for (uint32_t i = 0; i < meshList->GetSize(); ++i) {
        hd::graphics::Mesh *mesh = meshList->At(i);
        AddMesh(mesh);
    }
}


#define BUFFER_OFFSET(i) ((void*)(i))
void VertexBuffer::SetDefaultAttributeLocations(const Shader *shader) {
    const char * POSITION_ATTRIBUTE_NAME = "iPosition";
    const char * COLOR_ATTRIBUTE_NAME = "iDiffuseColor";
    const char * NORMAL_ATTRIBUTE_NAME = "iNormal";
    const char * TANGENT_ATTRIBUTE_NAME = "iTangent";
    const char * TEXCOORD_ATTRIBUTE_NAME = "iTexCoord";

    int32_t originLocation = shader->GetAttributeLocation(POSITION_ATTRIBUTE_NAME);
    int32_t colorLocation = shader->GetAttributeLocation(COLOR_ATTRIBUTE_NAME);
    int32_t normalLocation = shader->GetAttributeLocation(NORMAL_ATTRIBUTE_NAME);
    int32_t tangentLocation = shader->GetAttributeLocation(TANGENT_ATTRIBUTE_NAME);
    int32_t texCoordLocation = shader->GetAttributeLocation(TEXCOORD_ATTRIBUTE_NAME);

	for (uint32_t i = 0; i < m_buffers->Size(); ++i) {
        hd::graphics::VertexBufferInfo bufferData = m_buffers->At(i);

        glBindVertexArray(bufferData.vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, bufferData.vertexBufferObject);

        if (bufferData.triangleIndexBufferObject > 0 || bufferData.quadIndexBufferObject > 0) {
            if (bufferData.triangleIndexBufferObject > 0) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData.triangleIndexBufferObject);
            }
            if (bufferData.quadIndexBufferObject > 0) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData.quadIndexBufferObject);
            }
        }

        if (originLocation >= 0) {
            glEnableVertexAttribArray(originLocation);
            glVertexAttribPointer(originLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
        }

        if (colorLocation >= 0) {
            glEnableVertexAttribArray(colorLocation);
            glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
        }

        if (normalLocation >= 0) {
            glEnableVertexAttribArray(normalLocation);
            glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 7));
        }

        if (tangentLocation >= 0) {
            glEnableVertexAttribArray(tangentLocation);
            glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 10));
        }

        if (texCoordLocation >= 0) {
            glEnableVertexAttribArray(texCoordLocation);
            glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 13));
        }
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}


void VertexBuffer::Render() const {
	for (uint32_t i = 0; i < m_buffers->Size(); ++i) {
        hd::graphics::VertexBufferInfo bufferData = m_buffers->At(i);

        glBindVertexArray(bufferData.vertexArrayObject);
        if (bufferData.triangleIndexBufferObject > 0 || bufferData.quadIndexBufferObject > 0) {
            if (bufferData.triangleIndexBufferObject > 0) {
                glDrawRangeElements(GL_TRIANGLES, 0, bufferData.vertexCount - 1, bufferData.triangleCount * 3, GL_UNSIGNED_INT, NULL);
            }
        }
    }
    glBindVertexArray(0);
}
