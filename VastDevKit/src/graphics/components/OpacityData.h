#ifndef _VAST_GRAPHICS_COMPONENT_OPACITY_DATA_H_
#define _VAST_GRAPHICS_COMPONENT_OPACITY_DATA_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class OpacityData : public Component {

        private:

            float m_opacity;

            float m_opacityRange;


        protected:

            friend class ComponentPool<OpacityData>;

            OpacityData(unsigned int ID);


        public:

            void Reset();

            float GetOpacity() const;

            float GetRealOpacity() const;

            void SetOpacityRange(const float opacityRange);

            void SetOpacity(const float opacity);

            bool IsVisible() const;
            
        };
    }
}

#endif