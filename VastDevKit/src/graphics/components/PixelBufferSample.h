#ifndef _VAST_GRAPHICS_PIXEL_BUFFER_SAMPLE_H_
#define _VAST_GRAPHICS_PIXEL_BUFFER_SAMPLE_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    class Texture;
    
    namespace graphics {
        class PixelBufferSample : public Component {
            
        private:
            
            Color4f m_sampleColor;

            Vec2 m_uv;

            
        protected:
            
            friend class ComponentPool<PixelBufferSample>;
            
            PixelBufferSample(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void SetSampleColor(const Color4f &color) {
                m_sampleColor = color;
            }

            void SetSamplePosition(const Vec2 &uv) {
                m_uv = uv;
            }

            Color4f GetSampleColor() {
                return m_sampleColor;
            }
            
        };
    }
}

#endif