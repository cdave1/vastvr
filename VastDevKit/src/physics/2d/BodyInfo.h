
#ifndef _VAST_PHYSICS_2D_BODY_INFO_H_
#define _VAST_PHYSICS_2D_BODY_INFO_H_

#include <Vast/VastMath.h>

#define PHYSICS_SIZE_FACTOR 100.0f

namespace hd {
    namespace physics2d {
        enum class VertexConfiguration {
            Rectangle,      // DEFAULT. Ignores vertices, size used a vertices.
            Circle,         // Ignores vertices, size used as diameter
            TriangleFan,    // Vertices examined as if first vertex was pivot point,
                            // with subsequent vertices as points on a triangle fan.
            TriangleStrip,  
            Mesh,
            ConvexHull,
        };
        
        
        class BodyInfo {
        public:
            BodyInfo() :
                position(),
                aabb(0, 0, 1, 1),
                radius(0),
                friction(0),
                density(0),
                restitution(0),
                isStatic(false),
                isSensor(false),
                groupIndex(0),
                categoryBits(0),
                maskBits(0),
                configuration(VertexConfiguration::Rectangle)
            {}
            
            
            Vec3 position;
            
            AABB aabb;
            float radius;
            float friction;
            float density;
            float restitution;
            bool isStatic;
            bool isSensor;
            int groupIndex;
            int categoryBits;
            int maskBits;
            
            VertexConfiguration configuration;
            
        };
    }
}

#endif
