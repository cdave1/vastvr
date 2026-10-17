#ifndef _VAST_GRAPHICS_VIEWPORT_H_
#define _VAST_GRAPHICS_VIEWPORT_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"


template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Viewport : public Component {
        private:
            
            AABB m_aabb;
            
        protected:
            
            friend class ComponentPool<Viewport>;
            
            Viewport(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void SetAABB(const AABB &aabb);
            
            const AABB& GetAABB() const;
            
        };
    }
}

#endif
