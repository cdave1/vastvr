#ifndef _VAST_GRAPHICS_CURVE_H_
#define _VAST_GRAPHICS_CURVE_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

template<class T> class ComponentPool;

namespace hd
{
    namespace graphics
    {
        class Curve : public Component
        {
        private:
            
            Vec3 m_p1;
            
            Vec3 m_p2;
            
            Vec3 m_c1;
            
            Vec3 m_c2;
            
            
        protected:
            
            friend class ComponentPool<Curve>;
            
            Curve(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void AddCurve(const Vec3 &p1, const Vec3 &p2, const Vec3 &c1, const Vec3 &c2);
            
            Vec3 GetPoint(unsigned int index) const;
            
            Vec3 GetControlPoint(unsigned int index) const;
            
            
        };
    }
}

#endif
