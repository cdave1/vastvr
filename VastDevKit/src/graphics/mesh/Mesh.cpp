#include "Mesh.h"
#include "MeshMaterial.h"
#include "MeshVertexBuffer.h"

#include <list>

hd::graphics::Mesh::Mesh() : m_vertices(NULL),
    m_triangles(NULL),
    m_quads(NULL),
    m_meshMaterial(NULL),
    m_aabb() {
        m_vertices = new hd::Vector<Vertex>(32);
        m_lines = new hd::Vector<Size2>(32);
        m_triangles = new hd::Vector<Size3>(32);
        m_quads = new hd::Vector<Size4>(32);
        m_meshVertexBuffer = new MeshVertexBuffer();
}


hd::graphics::Mesh::~Mesh() {
    DeleteSafely(m_vertices);
    DeleteSafely(m_lines);
    DeleteSafely(m_triangles);
    DeleteSafely(m_quads);
    DeleteSafely(m_meshMaterial);
}


void hd::graphics::Mesh::Reset() {
    m_vertices->Clear();
    m_triangles->Clear();
    m_quads->Clear();
}


void hd::graphics::Mesh::AddVertex(Vertex vertex) {
    m_vertices->Add(vertex);
}


void hd::graphics::Mesh::AddLine(Size2 line) {
    m_lines->Add(line);
}


void hd::graphics::Mesh::AddTriangle(Size3 triangle) {
    m_triangles->Add(triangle);
}


void hd::graphics::Mesh::AddQuad(Size4 quad) {
    m_triangles->Add(Size3(quad[0], quad[1], quad[2]));
    m_triangles->Add(Size3(quad[0], quad[2], quad[3]));
//    m_quads->Add(quad);
}


void hd::graphics::Mesh::SetMeshMaterial(hd::graphics::MeshMaterial *mat) {
    m_meshMaterial = mat;
}


uint32_t hd::graphics::Mesh::CountVertices() const {
    return m_vertices->Size();
}


uint32_t hd::graphics::Mesh::CountLines() const {
    return m_lines->Size();
}


uint32_t hd::graphics::Mesh::CountTriangles() const {
    return m_triangles->Size();
}


uint32_t hd::graphics::Mesh::CountQuads() const {
    return m_quads->Size();
}


const hd::Vector<Vertex> * hd::graphics::Mesh::GetVertices() const {
    return m_vertices;
}


const hd::Vector<Size2> * hd::graphics::Mesh::GetLines() const {
    return m_lines;
}


const hd::Vector<Size3> * hd::graphics::Mesh::GetTriangles() const {
    return m_triangles;
}


const hd::Vector<Size4> * hd::graphics::Mesh::GetQuads() const {
    return m_quads;
}


const hd::graphics::MeshMaterial * hd::graphics::Mesh::GetMeshMaterial() const {
    return m_meshMaterial;
}


const AABB hd::graphics::Mesh::GetAABB() const {
    return m_aabb;
}


void hd::graphics::Mesh::GenerateNormals() {
	hdAssert(m_vertices->Size() != 0);
	hdAssert(m_triangles->Size() > 0 || m_quads->Size() > 0);
    
    //	initialize normals to zero:
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
		vertex->normal = Vec3(0.0f, 0.0f, 0.0f);
    }
    
    if (m_triangles->Size() > 0) {
        //	sum normals per-triangle:
		for (uint32_t i = 0; i < m_triangles->Size(); ++i) {
            Size3 triangle = m_triangles->At(i);
            Vertex *a = m_vertices->Item(triangle[0]);
            Vertex *b = m_vertices->Item(triangle[1]);
            Vertex *c = m_vertices->Item(triangle[2]);
            
            Vec3 ab = a->origin - b->origin;
            Vec3 ac = a->origin - c->origin;
            Vec3 norm = hd::cross(ac, ab);
            
            a->normal += norm;
            b->normal += norm;
            c->normal += norm;
        }
    }
    
    if (m_quads->Size() > 0) {
		for (uint32_t i = 0; i < m_quads->Size(); ++i) {
            Size4 quad = m_quads->At(i);
            Vertex *a = m_vertices->Item(quad[0]);
            Vertex *b = m_vertices->Item(quad[1]);
            Vertex *c = m_vertices->Item(quad[2]);
            Vertex *d = m_vertices->Item(quad[3]);
            
            Vec3 ab = a->origin - b->origin;
            Vec3 ac = a->origin - c->origin;
            Vec3 ad = a->origin - d->origin;
            Vec3 norm = hd::cross(hd::cross(ac, ab), ad);
            
            a->normal += norm;
            b->normal += norm;
            c->normal += norm;
            d->normal += norm;
        }
    }
    
    //	normalize per-vertex:
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        vertex->normal = hd::normalize(vertex->normal);
    }
}


// For each face, find the normal.
//
// Store this normal next to an index for each vertex in the face.  We can do this by storing an
// index of indices (eg. for the vertex at i, we need to take the average of all the normals of the
// vertexes at index j1, j2, ... , jn).
//
// Then, for each vertex we determine the normal by taking the average of all the normals for each vertex.
//
// Without a graph of adjacent triangle faces this process is turning out to be extremely hard.  We could
// achieve what we're trying to do with regular tesselation of the meshes.  The reason this is a problem
// is that each triangle may be a standalone -- it may have an adjacent triangle, but it may not
// necessarily share vertices with that triangle.
void hd::graphics::Mesh::GenerateSmoothNormals() {
    hdAssert(m_vertices->Size() != 0);
    hdAssert(m_triangles->Size() > 0 || m_quads->Size() > 0);

    //	initialize normals to zero:
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        vertex->normal = Vec3(0.0f, 0.0f, 0.0f);
    }

    Vec3 *triangleFaceNormals = new Vec3[m_triangles->Size()];
    Vec3 *quadFaceNormals = new Vec3[m_quads->Size()];

    if (m_triangles->Size() > 0) {
        //	sum normals per-triangle:
		for (uint32_t i = 0; i < m_triangles->Size(); ++i) {
            Size3 triangle = m_triangles->At(i);

            Vertex *a = m_vertices->Item(triangle[0]);
            Vertex *b = m_vertices->Item(triangle[1]);
            Vertex *c = m_vertices->Item(triangle[2]);

            Vec3 ab = a->origin - b->origin;
            Vec3 ac = a->origin - c->origin;
            Vec3 norm = hd::cross(ac, ab);

            triangleFaceNormals[i] = hd::normalize(norm);
        }
    }

    if (m_quads->Size() > 0) {
		for (uint32_t i = 0; i < m_quads->Size(); ++i) {
            Size4 quad = m_quads->At(i);
            Vertex *a = m_vertices->Item(quad[0]);
            Vertex *b = m_vertices->Item(quad[1]);
            Vertex *c = m_vertices->Item(quad[2]);
            Vertex *d = m_vertices->Item(quad[3]);

            Vec3 ab = a->origin - b->origin;
            Vec3 ac = a->origin - c->origin;
            Vec3 ad = a->origin - d->origin;
            Vec3 norm = hd::cross(hd::cross(ac, ab), ad);

            quadFaceNormals[i] = hd::normalize(norm);
        }
    }

	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *v = m_vertices->Item(i);

        std::list<Vec3> normals;

        // Find triangles that contain this vertex.
		for (uint32_t j = 0; j < m_triangles->Size(); ++j) {
            Size3 triangle = m_triangles->At(j);

            if (triangle[0] == i || triangle[1] == i || triangle[2] == i) {
                Vec3 triangleFaceNormal = triangleFaceNormals[j];
                normals.push_back(triangleFaceNormal);
            }
        }

		for (uint32_t j = 0; j < m_quads->Size(); ++j) {
            Size4 quad = m_quads->At(j);

            if (quad[0] == i || quad[1] == i || quad[2] == i || quad[3] == i) {
                Vec3 quadFaceNormal = quadFaceNormals[j];
                normals.push_back(quadFaceNormal);
            }
        }

        for (Vec3 n : normals) {
            v->normal += n;
        }
        v->normal = v->normal / normals.size();
    }

    //	normalize per-vertex:
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        vertex->normal = hd::normalize(vertex->normal);
    }
}



void hd::graphics::Mesh::GenerateTangents() {
	hdAssert(m_vertices->Size() != 0);
	hdAssert(m_triangles->Size() > 0 || m_quads->Size() > 0);
    
    //	initialize normals to zero:
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
		vertex->normal = Vec3(0.0f, 0.0f, 0.0f);
    }
    
    //	sum tangents per-triangle:
	for (uint32_t i = 0; i < m_triangles->Size(); ++i) {
        Size3 triangle = m_triangles->At(i);
		Vertex a = m_vertices->At(triangle[0]);
		Vertex b = m_vertices->At(triangle[1]);
		Vertex c = m_vertices->At(triangle[2]);
        
		Vec3 ba = b.origin - a.origin;
		Vec3 ca = c.origin - a.origin;
		
        Vec2 tba = b.texCoord - a.texCoord;
		Vec2 tca = c.texCoord - a.texCoord;
		float r = 1.0f / (tba[0] * tca[1] - tba[1] * tca[0]);
		Vec3 tang((tca[1] * ba[0] - tba[1] * ca[0]) * r,
                  (tca[1] * ba[1] - tba[1] * ca[1]) * r,
                  (tca[1] * ba[2] - tba[1] * ca[2]) * r);
        
		a.tangent += tang;
		b.tangent += tang;
		c.tangent += tang;
	}
    
    //	normalize per-vertex:
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        vertex->normal = hd::normalize(vertex->normal);
    }
    
	//vertexMask_ |= TANGENTS;
}

/*
void Mesh::transform(const mat4 &m) {
	mat3 nm = sml::transpose(mat3(sml::inverse(m)));
	for (IndexT i = 0; i < nVertices(); ++i) {
		vertices_[i].position = m * vertices_[i].position;
        
		vertices_[i].normal = nm * vertices_[i].normal;
		vertices_[i].normal.normalize();
        
		vertices_[i].tangent = nm * vertices_[i].tangent;
		vertices_[i].tangent.normalize();
	}
}


void Mesh::transformTexcoords(const mat3 &m) {
	for (IndexT i = 0; i < nVertices(); ++i) {
		vertices_[i].texcoord = m * vertices_[i].texcoord;
	}
}
*/
 
void hd::graphics::Mesh::FlipNormals() {
	hdAssert(m_vertices->Size() != 0);
	hdAssert(m_triangles->Size() > 0 || m_quads->Size() > 0);
    
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        vertex->normal = -vertex->normal;
    }
}


void hd::graphics::Mesh::FlipNormalsForYZ() {
	hdAssert(m_vertices->Size() != 0);
	hdAssert(m_triangles->Size() > 0 || m_quads->Size() > 0);
    
	for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);
        vertex->normal[1] = -vertex->normal[1];
        vertex->normal[2] = -vertex->normal[2];
    }
}


void hd::graphics::Mesh::FlipWinding() {
	hdAssert(m_vertices->Size() != 0);
	hdAssert(m_triangles->Size() > 0 || m_quads->Size() > 0);
    
    if (m_triangles->Size() > 0) {
        //	sum normals per-triangle:
		for (uint32_t i = 0; i < m_triangles->Size(); ++i) {
            Size3 *triangle = m_triangles->Item(i);
            uint32_t tmp = (*triangle)[0];
            (*triangle)[0] = (*triangle)[1];
            (*triangle)[1] = tmp;
        }
    }
    
    if (m_quads->Size() > 0) {
		for (uint32_t i = 0; i < m_quads->Size(); ++i) {
            Size4 *quad = m_quads->Item(i);
            uint32_t tmp = (*quad)[0];
            (*quad)[0] = (*quad)[1];
            (*quad)[1] = tmp;
            
            tmp = (*quad)[3];
            (*quad)[3] = (*quad)[4];
            (*quad)[4] = tmp;
        }
    }
}


void hd::graphics::Mesh::GenerateExtents() {
    if (m_vertices->Size() == 0) return;

    Vertex v0 = m_vertices->At(0);
    m_aabb.lower = v0.origin;
    m_aabb.upper = v0.origin;

    for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex vertex = m_vertices->At(i);
        m_aabb.lower = hd::min(m_aabb.lower, vertex.origin);
        m_aabb.upper = hd::max(m_aabb.upper, vertex.origin);
    }
}


void hd::graphics::Mesh::UploadMeshVertexBuffer() {
    m_meshVertexBuffer->SetMesh(this);
}


void hd::graphics::Mesh::ResetTextureCoords(const Size2 &size) {
    GenerateExtents();

    const float texRepeatX = 1.0f;
    const float texRepeatY = 1.0f;

    for (uint32_t i = 0; i < m_vertices->Size(); ++i) {
        Vertex *vertex = m_vertices->Item(i);

        float x = texRepeatX * ((vertex->origin[0] - m_aabb.lower[0])/size[0]);
        float y = texRepeatY * ((vertex->origin[1] - m_aabb.lower[1])/size[1]);

        vertex->texCoord.Set(x, y);
    }
}
