
#ifndef _VAST_PHYSICS_2D_DISTANCE_JOINT_INFO_H_
#define _VAST_PHYSICS_2D_DISTANCE_JOINT_INFO_H_

#include <Vast/VastMath.h>

namespace hd
{
    namespace physics2d
    {
        class DistanceJointInfo
        {
        public:
            
            DistanceJointInfo() :
                anchorA(),
                anchorB(),
                length(1),
                frequencyHz(0),
                dampingRatio(0)
            {}
            
            
            Vec3 anchorA;
            Vec3 anchorB;
            
            float length;
            float frequencyHz;
            float dampingRatio;
            
        };
    }
}

#endif
