#ifndef _VAST_GRAPHICS_SCREEN_COMPONENT_H_
#define _VAST_GRAPHICS_SCREEN_COMPONENT_H_

#include <vast/Vast.h>
#include "core/component/Component.h"


template<class T> class ComponentPool;

namespace hd
{
    namespace graphics
    {
        class ScreenData : public Component
        {
        private:
            
            AABB m_aabb;
            
            
        protected:
            
            friend class ComponentPool<ScreenData>;
            
            ScreenData(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void SetAABB(const AABB &aabb);
            
            const AABB& GetAABB() const;
            
            void RenderOrthographic() const;
            
        };
    }
}

#endif
