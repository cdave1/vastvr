#ifndef _VAST_GRAPHICS_MESH_DATA_H_
#define _VAST_GRAPHICS_MESH_DATA_H_

#include <string>
#include "core/collections/Collections.h"
#include <Vast/VastMath.h>
#include "math/Vertex.h"
#include "graphics/mesh/MeshPrototype.h"
#include "graphics/mesh/MeshInstanceData.h"

namespace hd {
    namespace graphics {
        class Mesh;
        class Shader;
        
        class MeshData {
        private:
            
            hd::Vector<hd::graphics::Mesh *> * m_meshes;
            
            MeshPrototype m_prototype;

            uint32_t m_id;

            AABB m_aabb;

            
        public:
            
            hd::Vector<hd::graphics::Mesh *> * GetMeshes() const;

            MeshPrototype GetPrototype() const {
                return m_prototype;
            }

            const AABB GetAABB() const;

            void UploadMeshData();

            void SetDefaultAttributeLocations(const Shader *shader) const;

            void SetInstancedAttributeLocations(const Shader *shader) const;

            void RenderDepthInstanced(const Matrix *cameraData, const Matrix *instanceData, const uint32_t count) const;

            void RenderInstanced(const MeshInstanceData *renderData, const uint32_t transformCount) const;

            void RenderLines() const;

            void Render() const;
            
            
        private:
            
            friend class MeshLoader;
            
            MeshData(uint32_t id);
            
            ~MeshData();

            uint32_t GetId() const {
                return m_id;
            }

            void SetPrototype(const MeshPrototype &prototype) {
                m_prototype = prototype;
            }
            
            void AddMesh(hd::graphics::Mesh *mesh);
            
            bool ContainsMeshPath(const std::string &path) const;

            void GenerateExtents();
            
        };
    }
}

#endif
