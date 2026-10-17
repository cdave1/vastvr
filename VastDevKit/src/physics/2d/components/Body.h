#ifndef _VAST_PHYSICS_COMPONENTS_BODY_H_
#define _VAST_PHYSICS_COMPONENTS_BODY_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

class b2World;
class b2Body;

namespace hd
{
    namespace physics2d
    {
        extern const char *BodyUpdatedMessage;
        
        class World;
        class BodyInfo;
        
        class Body : public Component
        {
        private:
            
            b2Body *m_physicsBody;
            
            bool m_isStatic;
            
            
        public:
            
            void Reset();
            
            void CreatePhysicsBody(physics2d::World *world, physics2d::BodyInfo bodyInfo);
            
            const b2Body * GetPhysicsBody() const;
            
            Vec3 GetPosition() const;
            
            float GetAngle() const;
            
            
        protected:
            
            friend class ComponentPool<Body>;
            
            Body(unsigned int ID);
            
        };
    }
}

#endif
