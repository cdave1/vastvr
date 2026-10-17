#include "VertexBuffer.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/MeshData.h"
#include "graphics/components/Polygon.h"
#include "graphics/components/Shader.h"
#include "graphics/gl/GraphicsInterface.h"

using namespace hd::graphics;


VertexBuffer::VertexBuffer(unsigned int ID) : Component(ID),
m_originLocation(0),
m_colorLocation(0),
m_normalLocation(0),
m_tangentLocation(0),
m_texCoordLocation(0),
m_usage(GL_STATIC_DRAW),
m_buffers(NULL) {
    m_buffers = new hd::Vector<hd::graphics::VertexBufferInfo>(8);
    Reset();
}


void VertexBuffer::Reset() {
    for (int i = 0; i < m_buffers->Size(); ++i) {
        hd::graphics::VertexBufferInfo bufferData = m_buffers->At(i);

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


void VertexBuffer::SetFromSphere(const Polygon *sphere) {
    //
    // HACK HACK HACK
    //
    // Sphere is setup as a list of Quads.  This wastes a large amount of vertex memory.
    //
    SetFromPolygon(sphere);
    m_buffers->Item(0)->primitive = GL_QUADS;
}


void VertexBuffer::SetFromPolygon(const Polygon *polygon) {
    const hd::Vector<Vertex> *vertices = polygon->GetVertices();

    hd::graphics::VertexBufferInfo bufferData;
    bufferData.vertexCount = vertices->Size();

    if (vertices->Size() == 3) {
		bufferData.primitive = GL_TRIANGLES;
	} else if (vertices->Size() == 4) {
		bufferData.primitive = GL_QUADS;
	}
	else
	{
//#if TARGET_GL_OPENGL == 1
//		bufferData.primitive = GL_POLYGON;
//#else
		bufferData.primitive = GL_TRIANGLE_FAN;
//#endif
	}

    Vertex *buffer = (Vertex *)calloc(1, sizeof(Vertex) * bufferData.vertexCount);
    for (int i = 0; i < vertices->Size(); ++i) {
        Vertex vertex = vertices->At(i);
        buffer[i] = vertex;
	}

    glGenBuffers(1, &bufferData.vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, bufferData.vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * bufferData.vertexCount, buffer, m_usage);

    FreeSafely(buffer);

    m_buffers->Add(bufferData);
}


void VertexBuffer::AddMesh(const hd::graphics::Mesh *meshData) {
    //
    // HACK HACK HACK
    //
    // Null component continually receives new buffers when this funtion is called.
    // We check if the component is a null component in this case.  Need a better way of
    // preventing this!!!
    //
    if (IsNullComponent()) {
        return;
    }

    const hd::Vector<Vertex> *vertices = meshData->GetVertices();
    hd::graphics::VertexBufferInfo bufferData;
    bufferData.vertexCount = vertices->Size();
    bufferData.primitive = GL_TRIANGLE_FAN;

    Vertex *buffer = (Vertex *)malloc(sizeof(Vertex) * bufferData.vertexCount);
    for (int i = 0; i < vertices->Size(); ++i) {
        Vertex vertex = vertices->At(i);
        buffer[i] = vertex;
	}

    if (bufferData.vertexBufferObject == 0) {
        glGenBuffers(1, &bufferData.vertexBufferObject);
    }

    glBindBuffer(GL_ARRAY_BUFFER, bufferData.vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * bufferData.vertexCount, buffer, m_usage);
    FreeSafely(buffer);

    const hd::Vector<Size3> *triangles = meshData->GetTriangles();
    const hd::Vector<Size4> *quads = meshData->GetQuads();

    if (triangles->Size() > 0) {
        bufferData.triangleCount = triangles->Size();

        Size3 *indexBuffer = (Size3 *)malloc(sizeof(Size3) * bufferData.triangleCount);
        for (int i = 0; i < triangles->Size(); ++i) {
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

    if (quads->Size() > 0) {
        bufferData.quadCount = quads->Size();

        Size4 *indexBuffer = (Size4 *)malloc(sizeof(Size4) * bufferData.quadCount);
        for (int i = 0; i < quads->Size(); ++i) {
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
    for (int i = 0; i < meshList->GetSize(); ++i) {
        hd::graphics::Mesh *mesh = meshList->At(i);
        AddMesh(mesh);
    }
}


void VertexBuffer::SetDefaultAttributeLocations(const Shader *shader) {
    const std::string POSITION_ATTRIBUTE_NAME = "iPosition";
    const std::string COLOR_ATTRIBUTE_NAME = "iDiffuseColor";
    const std::string NORMAL_ATTRIBUTE_NAME = "iNormal";
    const std::string TANGENT_ATTRIBUTE_NAME = "iTangent";
    const std::string TEXCOORD_ATTRIBUTE_NAME = "iTexCoord";

    int32_t originLocation = shader->GetAttributeLocation(POSITION_ATTRIBUTE_NAME);
    int32_t colorLocation = shader->GetAttributeLocation(COLOR_ATTRIBUTE_NAME);
    int32_t normalLocation = shader->GetAttributeLocation(NORMAL_ATTRIBUTE_NAME);
    int32_t tangentLocation = shader->GetAttributeLocation(TANGENT_ATTRIBUTE_NAME);
    int32_t texCoordLocation = shader->GetAttributeLocation(TEXCOORD_ATTRIBUTE_NAME);

    this->SetOriginAttributeLocation(originLocation);
    this->SetColorAttributeLocation(colorLocation);
    this->SetNormalAttributeLocation(normalLocation);
    this->SetTangentAttributeLocation(tangentLocation);
    this->SetTexCoordAttributeLocation(texCoordLocation);

    shader->BindAttributeLocation(POSITION_ATTRIBUTE_NAME);
    shader->BindAttributeLocation(COLOR_ATTRIBUTE_NAME);
    shader->BindAttributeLocation(NORMAL_ATTRIBUTE_NAME);
    shader->BindAttributeLocation(TANGENT_ATTRIBUTE_NAME);
    shader->BindAttributeLocation(TEXCOORD_ATTRIBUTE_NAME);
}


void VertexBuffer::SetOriginAttributeLocation(const int32_t location) {
    m_originLocation = location;
}


void VertexBuffer::SetColorAttributeLocation(const int32_t location) {
    m_colorLocation = location;
}


void VertexBuffer::SetNormalAttributeLocation(const int32_t location) {
    m_normalLocation = location;
}


void VertexBuffer::SetTangentAttributeLocation(const int32_t location) {
    m_tangentLocation = location;
}


void VertexBuffer::SetTexCoordAttributeLocation(const int32_t location) {
    m_texCoordLocation = location;
}

/*
 Vec4 origin;
 Color4f color;
 Vec3 normal;
 Vec3 tangent;
 Vec2 texCoord;
 */

#define BUFFER_OFFSET(i) ((void*)(i))

void VertexBuffer::Render() {
    for (int i = 0; i < m_buffers->Size(); ++i) {
        hd::graphics::VertexBufferInfo bufferData = m_buffers->At(i);

        glBindBuffer(GL_ARRAY_BUFFER, bufferData.vertexBufferObject);
        hdglError("");

        if (m_originLocation >= 0) {
          //  glEnableVertexAttribArray(m_originLocation);
            if (hdglError("")) {
                printf("");
            }
            glVertexAttribPointer(m_originLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
            if (hdglError("")) {
                printf("");
            }
        }

        if (m_colorLocation >= 0) {
            glEnableVertexAttribArray(m_colorLocation);
            glVertexAttribPointer(m_colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
        }
        hdglError("");

        if (m_normalLocation >= 0) {
            glEnableVertexAttribArray(m_normalLocation);
            glVertexAttribPointer(m_normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 7));
        }
        hdglError("");

        if (m_tangentLocation >= 0) {
            glEnableVertexAttribArray(m_tangentLocation);
            glVertexAttribPointer(m_tangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 10));
        }
        hdglError("");

        if (m_texCoordLocation >= 0) {
            glEnableVertexAttribArray(m_texCoordLocation);
            glVertexAttribPointer(m_texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 13));
        }
        hdglError("");

        if (bufferData.triangleIndexBufferObject > 0 || bufferData.quadIndexBufferObject > 0) {
            if (bufferData.triangleIndexBufferObject > 0) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData.triangleIndexBufferObject);
                glDrawElements(GL_TRIANGLES, bufferData.triangleCount * 3, GL_UNSIGNED_INT, ((char *)NULL));
            }
            if (bufferData.quadIndexBufferObject > 0) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData.quadIndexBufferObject);
                glDrawElements(GL_QUADS, bufferData.quadCount * 4, GL_UNSIGNED_INT, ((char *)NULL));
            }
        } else {
            glDrawArrays(bufferData.primitive, 0, bufferData.vertexCount);
        }
        hdglError("");

        if (m_originLocation >= 0) {
            glDisableVertexAttribArray(m_originLocation);
        }
        hdglError("");

        if (m_normalLocation >= 0) {
            glDisableVertexAttribArray(m_normalLocation);
        }
        hdglError("");

        if (m_texCoordLocation >= 0) {
            glDisableVertexAttribArray(m_texCoordLocation);
        }
        hdglError("");

        if (m_tangentLocation >= 0) {
            glDisableVertexAttribArray(m_tangentLocation);
        }
        hdglError("");

        if (m_colorLocation >= 0) {
            glDisableVertexAttribArray(m_colorLocation);
        }
        hdglError("");
        hdglError("");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        hdglError("");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        hdglError("");
    }
}
