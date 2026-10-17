
#ifndef _VAST_PHYSICS_2D_PRISMATIC_JOINT_INFO_H_
#define _VAST_PHYSICS_2D_PRISMATIC_JOINT_INFO_H_

#include <Vast/VastMath.h>

namespace hd
{
    namespace physics2d
    {
        class PrismaticJointInfo
        {
        public:
            
            PrismaticJointInfo() :
                anchorPosition(),
                axisPosition(),
                enableMotor(false),
                enableLimit(false),
                motorSpeed(0),
                lowerTranslation(0),
                upperTranslation(0),
                maxMotorForce(0)
            {}
            
            
            Vec3 anchorPosition;
            Vec3 axisPosition;
            
            bool enableMotor;
            bool enableLimit;
            
            float motorSpeed;
            
            float lowerTranslation;
            float upperTranslation;
            float maxMotorForce;
            
        };
    }
}

#endif
