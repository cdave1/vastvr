#ifndef _VAST_INPUT_POINTER_DRAG_H_
#define _VAST_INPUT_POINTER_DRAG_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/pointer/PointerCode.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;
        
        namespace pointer {
            class Drag : public Component {
            private:
                
                hd::input::pointer::Control m_control;

                hd::input::pointer::Button m_button;
                
                
            private:
                
                friend class hd::input::EventQueue;

                void SetControl(hd::input::pointer::Control control);

                void SetButton(hd::input::pointer::Button button);
                
                
            protected:
                
                friend class ComponentPool<Drag>;
                
                Drag(unsigned int ID);
                
                
            public:

                hd::input::pointer::Control GetControl() const;

                hd::input::pointer::Button GetButton() const;
                
                void PrintDebugInfo() const;
                
                
            };
        }
    }
}

#endif