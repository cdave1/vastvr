
#ifndef _VAST_PHYSICS_2D_REVOLUTE_JOINT_INFO_H_
#define _VAST_PHYSICS_2D_REVOLUTE_JOINT_INFO_H_

#include <Vast/VastMath.h>

namespace hd
{
    namespace physics2d
    {
        class RevoluteJointInfo
        {
        public:
            
            RevoluteJointInfo() :
                position(),
                enableMotor(false),
                enableLimit(false),
                motorSpeed(0),
                lowerAngle(0),
                upperAngle(0),
                maxMotorTorque(0)
            {}
            
            
            Vec3 position;
            
            bool enableMotor;
            bool enableLimit;
            
            float motorSpeed;
            float lowerAngle;
            float upperAngle;
            float maxMotorTorque;
            
        };
    }
}

#endif
