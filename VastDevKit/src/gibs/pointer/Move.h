#ifndef _VAST_INPUT_POINTER_MOVE_H_
#define _VAST_INPUT_POINTER_MOVE_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/pointer/PointerCode.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;
        
        namespace pointer {
            class Move : public Component {
            private:
                
                hd::input::pointer::Control m_control;
                
                
            private:
                
                friend class hd::input::EventQueue;

                void SetControl(hd::input::pointer::Control control);
                
                
            protected:
                
                friend class ComponentPool<Move>;
                
                Move(unsigned int ID);
                
                
            public:

                hd::input::pointer::Control GetControl() const;
                
                void PrintDebugInfo() const;
                
                
            };
        }
    }
}

#endif