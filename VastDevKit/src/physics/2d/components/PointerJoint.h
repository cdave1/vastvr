
#ifndef _VAST_PHYSICS_COMPONENTS_POINTER_JOINT_H_
#define _VAST_PHYSICS_COMPONENTS_POINTER_JOINT_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

class b2World;
class b2MouseJoint;

namespace hd
{
    namespace physics2d
    {
        class World;
        class Body;
        class PointerJointInfo;
        
        class PointerJoint : public Component
        {
        private:
            
            b2MouseJoint *m_joint;
            
            
        public:
            
            void Reset();
            
            void CreateJoint(physics2d::World *world,
                             physics2d::PointerJointInfo jointInfo,
                             physics2d::Body *first,
                             physics2d::Body *second);
            
            void UpdateJoint(const Vec3 &target);
            
            void DestroyJoint(physics2d::World *world);
            
            const b2MouseJoint * GetJoint() const;
            
            
        protected:
            
            friend class ComponentPool<PointerJoint>;
            
            PointerJoint(unsigned int ID);
            
        };
    }
}

#endif
