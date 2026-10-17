#ifndef _VAST_GRAPHICS_MESH_VERTEX_BUFFER_H_
#define _VAST_GRAPHICS_MESH_VERTEX_BUFFER_H_

#include <Vast/VastMath.h>
#include <graphics/mesh/MeshInstanceData.h>

namespace hd {
    namespace graphics {
        class Shader;
        class Mesh;
        class MeshData;
        
        class MeshVertexBuffer {
        private:

            uint32_t m_usage;

            uint32_t m_vertexBufferObject;

            uint32_t m_vertexArrayObject;

            uint32_t m_lineIndexBufferObject;

            uint32_t m_triangleIndexBufferObject;

            uint32_t m_quadIndexBufferObject;

            mutable uint32_t m_transformBufferObject;

            uint32_t m_depthVertexBufferObject;

            uint32_t m_depthVertexArrayObject;

            mutable uint32_t m_depthPassBufferObject;

            uint32_t m_primitive;

            uint32_t m_vertexCount;

            uint32_t m_lineCount;

            uint32_t m_triangleCount;
            
            uint32_t m_quadCount;

            uint32_t m_indexSize;

            
        protected:
            friend class Mesh;

            MeshVertexBuffer();

            void SetDepthPassAttributeLocations() const;
            
            
        public:
            
            void Reset();

            void SetStatic();

            void SetDynamic();
            
            void SetMesh(const hd::graphics::Mesh *mesh);
            
            void SetDefaultAttributeLocations(const Shader *shader) const;

            void SetInstancedAttributeLocations(const Shader *shader) const;

            void RenderDepthInstanced(const Matrix *cameraData, const Matrix *instanceData, const uint32_t count) const;

            void RenderInstanced(const MeshInstanceData *renderData, const uint32_t transformCount) const;

            void RenderLines() const;

            void Render() const;
            
        };
    }
}

#endif
