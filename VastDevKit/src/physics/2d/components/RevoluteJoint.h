
#ifndef _VAST_PHYSICS_COMPONENTS_REVOLUTEJOINT_H_
#define _VAST_PHYSICS_COMPONENTS_REVOLUTEJOINT_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

class b2World;
class b2Joint;

namespace hd
{
    namespace physics2d
    {
        class World;
        class BodyInfo;
        
        class RevoluteJoint : public Component
        {
        private:
            
            b2Joint *m_joint;
            
            
        public:
            
            void Reset();
            
            void CreateJoint(physics2d::World *world,
                             physics2d::RevoluteJointInfo jointInfo,
                             physics2d::Body *first,
                             physics2d::Body *second);
            
            const b2Joint * GetJoint() const;
            
            
        protected:
            
            friend class ComponentPool<RevoluteJoint>;
            
            RevoluteJoint(unsigned int ID);
            
        };
    }
}

#endif
