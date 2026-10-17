#ifndef _VAST_GRAPHICS_COMPONENT_POLYGON_RENDERER_H_
#define _VAST_GRAPHICS_COMPONENT_POLYGON_RENDERER_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Polygon;
        class Material;
        
        class PolygonRenderer : public Component
        {
        private:
            
            unsigned long m_currentFrame;
            
            
        protected:
            
            friend class ComponentPool<PolygonRenderer>;
            
            PolygonRenderer(unsigned int ID);
            
            
        public:
            
            void Reset();
            
            void Update();
            
            void SetCurrentFrame(unsigned long currentFrame);
            
			void RenderSphere(const hd::graphics::Polygon *polygon) const;
            
			void RenderLines(const hd::graphics::Polygon *polygon) const;
            
			void RenderLineStrip(const hd::graphics::Polygon *polygon) const;
            
			void RenderSolid(const hd::graphics::Polygon *polygon, const Color4f &color = Color4f(1.0f, 1.0f, 1.0f, 1.0f)) const;
            
			void RenderNormals(const hd::graphics::Polygon *polygon) const;
            
			void RenderPolygon(const hd::graphics::Polygon *polygon) const;
            
        };
    }
}

#endif

