#ifndef _VAST_GRAPHICS_COMPONENT_COLLISION_VOLUME_H_
#define _VAST_GRAPHICS_COMPONENT_COLLISION_VOLUME_H_

#include "core/component/Component.h"
#include "core/component/ComponentPool.h"

namespace hd {
    template<class T> class ComponentPool;
}

namespace hd {
    namespace graphics {
        class CollisionVolume : public hd::Component {
        private:

            AABB m_aabb;

            Vec3 m_nodeSize;

            Size2 m_nodeCount;

        protected:

            friend class hd::ComponentPool<CollisionVolume>;

            CollisionVolume(unsigned int ID);


        public:

            void SetAABB(const AABB &aabb);
            
            void SetNodeSize(const Vec3 &size);

            const Vec3& GetNodeSize() const;
            
            void SetNodeCount(const Size2 &count);

            Size2 GetNodeCount() const;

            ///
            /// Returns indexes and centers of nodes between the two collision locations.
            ///
            /// The indexes will form a box within the collision volume.
            ///
            uint32_t IndexesOf(const Collision3 &start,
                               const Collision3 &end,
                               Size2 *indexes,
                               Vec3 *centers,
                               const uint32_t count);

            Vec3 CenterOf(const Size2 &index);

            ///
            /// Get a normalized center of the collision based on the current node size.
            ///
            Vec3 GetCollisionCenter(const Collision3 &collision);

            Size3 GetCollisionIndex(const Collision3 &collision);
            
            bool IndexOf(const Collision3 &collision, Size2 *index, Vec3 *center = nullptr);
            
        };
    }
}

#endif