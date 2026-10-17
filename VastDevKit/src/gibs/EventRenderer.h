#ifndef _VAST_INPUT_EVENT_RENDERER_H_
#define _VAST_INPUT_EVENT_RENDERER_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd
{
    namespace input
    {
        namespace pointer
        {
            class Position;
        }
        
        class EventRenderer : public Component
        {   
        protected:
            
            friend class ComponentPool<EventRenderer>;
            
            EventRenderer(unsigned int ID);
            
            
        public:
            
            static void RenderCamera();
            
            static void RenderPointerSelect(const pointer::Position *pointer);
            
            static void RenderPointerDrag(const pointer::Position *pointer);
            
            static void RenderPointerRelease(const pointer::Position *pointer);
            
        };
    }
}

#endif

