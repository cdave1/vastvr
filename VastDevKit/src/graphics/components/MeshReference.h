#ifndef _VAST_GRAPHICS_COMPONENT_MESH_REFERENCE_H_
#define _VAST_GRAPHICS_COMPONENT_MESH_REFERENCE_H_

#include <Vast/VastMath.h>
#include <graphics/mesh/MeshInstanceData.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class MeshData;

        class MeshReference : public Component {
            
        private:

            MeshData *m_mesh;

            
        protected:
            
            friend class ComponentPool<MeshReference>;
            
            MeshReference(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void SetMeshReference(MeshData *meshData) {
                m_mesh = meshData;
            }

            const MeshData * GetMeshData() const {
                return m_mesh;
            }

            void RenderDepthInstanced(const Matrix *cameraData, const Matrix *instanceData, const uint32_t count) const;

            void RenderInstanced(const MeshInstanceData *instanceData, const uint32_t count) const;

            void RenderLines() const;

            void Render() const;
            
        };
    }
}

#endif