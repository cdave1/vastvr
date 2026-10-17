#ifndef _VAST_GRAPHICS_VISIBLITY_H_
#define _VAST_GRAPHICS_VISIBLITY_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"


template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Visibility : public Component {
        private:
            
            uint32_t m_currentFrame;
            
        protected:
            
            friend class ComponentPool<Visibility>;
            
            Visibility(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void SetCurrentFrame(const uint32_t &frame);
            
            const uint32_t GetCurrentFrame() const;
        };
    }
}

#endif
