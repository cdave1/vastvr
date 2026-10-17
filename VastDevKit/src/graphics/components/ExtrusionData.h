#ifndef _VAST_GRAPHICS_COMPONENT_EXTRUSION_DATA_H_
#define _VAST_GRAPHICS_COMPONENT_EXTRUSION_DATA_H_

#include <Vast/VastMath.h>
#include "core/Collections/Collections.h"
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd
{
    namespace graphics
    {
        class Polygon;
        
        class ExtrusionData : public Component
        {
        private:
            
            float m_depth;
            
            bool m_reflect;
                        
            Vector<Vec2> *m_textureCoords;
            
            
        protected:
            
            friend class ComponentPool<ExtrusionData>;
            
            ExtrusionData(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            float GetDepth() const;
            
            void SetDepth(float depth);
            
            void GenerateTextureCoords(const Polygon *polygon);
            
            const Vector<Vec2> * GetTextureCoords() const;
            
        };
    }
}

#endif

