#ifndef _VAST_GRAPHICS_LIGHT_MODEL_H_
#define _VAST_GRAPHICS_LIGHT_MODEL_H_

#include <Vast/Vast.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class LightModel : public Component
        {
        private:
            
            Color4f m_ambient;
            
        public:
            
            void Reset();
            
            const Color4f & GetAmbient() const;
            
            void SetAmbient(const Color4f &ambient);
            
            
        protected:
            
            friend class ComponentPool<LightModel>;
            
            LightModel(unsigned int ID);
            
            
        };
    }
}

#endif