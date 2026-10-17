#ifndef _VAST_GRAPHICS_COMPONENT_COLLISION_DATA_H_
#define _VAST_GRAPHICS_COMPONENT_COLLISION_DATA_H_

#include "core/component/Component.h"
#include "math/AABB.h"
#include "math/Collision3.h"
#include "math/Matrix.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class MeshData;

        ///
        /// A reference to a mesh for use in collision testing.
        ///
        class CollisionData : public Component {

        private:

            hd::graphics::MeshData *m_meshes;

            uint32_t m_currentFrame;

            float m_distanceToCamera;

            Collision3 m_collision;

            Matrix m_transformMatrix;

            std::function<Matrix()> m_onGetTransform;



        protected:

            friend class ComponentPool<hd::graphics::CollisionData>;

            CollisionData(unsigned int ID);


        public:

            void Reset();

            const hd::graphics::MeshData * GetMeshData() const;

            void SetMeshData(hd::graphics::MeshData *meshes);

            AABB GetAABB() const;

            void LoadMesh(const std::string &path);

            void SetCurrentFrame(const uint32_t &frame);

            uint32_t GetCurrentFrame() const;

            void SetDistanceToCamera(const float distance);

            float GetDistanceToCamera() const;

            void SetCollision(const Collision3 &collision) {
                m_collision = collision;
            }

            Collision3 GetCollision() const {
                return m_collision;
            }

            void OnGetTransform(std::function<Matrix()> getTransform) {
                m_onGetTransform = getTransform;
            }

            Matrix GetTransformMatrix() const {
                if (m_onGetTransform) {
                    return m_onGetTransform();
                } else {
                    return Matrix::Identity();
                }
            }
            
        };
    }
}

#endif
