#ifndef _VAST_GRAPHICS_VERTEX_BUFFER_H_
#define _VAST_GRAPHICS_VERTEX_BUFFER_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>
#include "core/collections/Vector.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        typedef struct VertexBufferInfo {
        public:

            VertexBufferInfo() :
                vertexBufferObject(0),
                vertexArrayObject(0),
                triangleIndexBufferObject(0),
                quadIndexBufferObject(0),
                primitive(0),
                vertexCount(0),
                triangleCount(0),
                quadCount(0)
            {}

            ~VertexBufferInfo() {}
            
            uint32_t vertexBufferObject;

            uint32_t vertexArrayObject;

            uint32_t triangleIndexBufferObject;

            uint32_t quadIndexBufferObject;

            uint32_t primitive;

            uint32_t vertexCount;

            uint32_t triangleCount;

            uint32_t quadCount;

        } VertexBufferInfo;
    }
}

namespace hd {
    namespace graphics {
        class Polygon;
        class Shader;
        class Mesh;
        class MeshData;
        
        class VertexBuffer : public Component {
        private:

            uint32_t m_usage;

            hd::Vector<hd::graphics::VertexBufferInfo> *m_buffers;

            
        protected:
            
            friend class ComponentPool<VertexBuffer>;
            
            VertexBuffer(unsigned int ID);
            
            
        public:
            
            void Reset();

            void SetStatic();

            void SetDynamic();
            
            void SetFromMesh(const hd::graphics::Mesh *mesh);

            void AddMesh(const hd::graphics::Mesh *meshData);

            void AddMeshes(const hd::graphics::MeshData *meshes);
            
            void SetDefaultAttributeLocations(const Shader *shader);
            
            void Render() const;
            
        };
    }
}

#endif
