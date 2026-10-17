
#ifndef _VAST_PHYSICS_2D_POINTER_JOINT_INFO_H_
#define _VAST_PHYSICS_2D_POINTER_JOINT_INFO_H_

#include <Vast/VastMath.h>

namespace hd
{
    namespace physics2d
    {
        class PointerJointInfo
        {
        public:
            
            PointerJointInfo() :
                collideConnected(true),
                target(),
                targetOffset(),
                frequencyHz(30.0f),
                dampingRatio(1.0f),
                forceMultiplier(100.0f)
            {}
            
            
            bool collideConnected;
            
            Vec3 target;
            Vec3 targetOffset;
            
            float frequencyHz;
            float dampingRatio;
            float forceMultiplier;
            
        };
    }
}

#endif
