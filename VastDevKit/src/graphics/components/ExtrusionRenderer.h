#ifndef _VAST_GRAPHICS_COMPONENT_EXTRUSION_RENDERER_H_
#define _VAST_GRAPHICS_COMPONENT_EXTRUSION_RENDERER_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace graphics {
        class Polygon;
        class ExtrusionData;
        class Material;
        
        class ExtrusionRenderer : public Component {
        protected:
            
            friend class ComponentPool<ExtrusionRenderer>;
            
            ExtrusionRenderer(unsigned int ID);
            
            
        public:
            
            void RenderFlatQuad(const Polygon *polygon,
                                const ExtrusionData *extrusionData,
                                const Material *material);
            
        };
    }
}

#endif

