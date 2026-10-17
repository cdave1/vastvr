#ifndef _VAST_GRAPHICS_CURVE_RENDERER_H_
#define _VAST_GRAPHICS_CURVE_RENDERER_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd
{
    namespace graphics
    {
        class Curve;
        
        class CurveRenderer : public Component
        {
        public:

            void RenderCurve(const Curve *curve);
            
            void RenderCurve(Vec3 p1, Vec3 p2, Vec3 c1, Vec3 c2);
            
            
        protected:
            
            friend class ComponentPool<CurveRenderer>;
            
            CurveRenderer(unsigned int ID);
            
            
        };
    }
}

#endif