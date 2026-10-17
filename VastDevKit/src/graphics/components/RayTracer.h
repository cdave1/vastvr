#ifndef _VAST_GRAPHICS_RAY_TRACER_COMPONENT_H_
#define _VAST_GRAPHICS_RAY_TRACER_COMPONENT_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Projection;
        class LookAtData;

        class RayTracer : public Component {

        protected:
            
            friend class ComponentPool<RayTracer>;
            
            RayTracer(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            Vec3 PointerToWorld(const Vec3 &pointer,
                                const AABB &viewport,
                                const Matrix &projection,
                                const Matrix &modelViewMatrix) const;
            
        };
    }
}

#endif
