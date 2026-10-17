
#ifndef _VAST_PHYSICS_2D_WORLD_INFO_H_
#define _VAST_PHYSICS_2D_WORLD_INFO_H_

#include <Vast/VastMath.h>

namespace hd
{
    namespace physics2d
    {
        class WorldInfo
        {
        public:
            
            WorldInfo() :
                gravity(0.0f, -10.0f, 0.0f),
                allowSleep(false),
                fixedFrameDuration(0.016f),
                simulationRateMultiplier(1.0f),
                iterationsPerUpdate(5)
            {}
            
            
            Vec3 gravity;
            bool allowSleep;
            float fixedFrameDuration;
            float simulationRateMultiplier;
            unsigned int iterationsPerUpdate;
            
        };
    }
}

#endif
