#ifndef _VAST_GRAPHICS_MESH_H_
#define _VAST_GRAPHICS_MESH_H_

#include <string>
#include "core/collections/Collections.h"
#include <Vast/VastMath.h>

namespace hd {
    namespace graphics {
        class MeshMaterial;
        class MeshVertexBuffer;

        enum class Primitive {
            POINTS,
            LINES,
            LINE_LOOP,
            LINE_STRIP,
            TRIANGLES,
            TRIANGLE_STRIP,
            TRIANGLE_FAN,
            QUADS,
            QUAD_STRIP,
            POLYGON
        };

        class Mesh {
        private:
            
            hd::Vector<Vertex> *m_vertices;

            hd::Vector<Size2> *m_lines;
            
            hd::Vector<Size3> *m_triangles;
            
            hd::Vector<Size4> *m_quads;
            
            AABB m_aabb;
            
            MeshMaterial *m_meshMaterial;

            MeshVertexBuffer *m_meshVertexBuffer;
            
            
        public:
            
            const Vector<Vertex> * GetVertices() const;

            const Vector<Size2> * GetLines() const;
            
            const Vector<Size3> * GetTriangles() const;
            
            const Vector<Size4> * GetQuads() const;
            
            const MeshMaterial * GetMeshMaterial() const;

            const AABB GetAABB() const;
            
            void FlipNormals();
            
            void FlipNormalsForYZ();
            
            void FlipWinding();
            
            void GenerateNormals();

            void GenerateSmoothNormals();

            void UploadMeshVertexBuffer();

            void ResetTextureCoords(const Size2 &size);
            
            
        private:
            
            friend class MeshLoader;
            friend class MeshData;
            
            Mesh();
            
            ~Mesh();
            
            void Reset();
            
            void AddVertex(Vertex vertex);

            void AddLine(Size2 line);
            
            void AddTriangle(Size3 triangle);
            
            void AddQuad(Size4 quad);
            
            void SetMeshMaterial(MeshMaterial *mat);
            
            void CreateSphere(float radius, const Vec2 &segments);
            
            void GenerateTangents();
            
            uint32_t CountVertices() const;

            uint32_t CountLines() const;
            
            uint32_t CountTriangles() const;
            
            uint32_t CountQuads() const;
            
            void GenerateExtents();

            const MeshVertexBuffer * GetMeshVertexBuffer() const {
                return m_meshVertexBuffer;
            }
            
        };
    }
}


#endif