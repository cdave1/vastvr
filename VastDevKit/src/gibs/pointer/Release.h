
/**
 * Component representing a pointer release event. e.g:
 *   Mouse Button Up
 *   Touch Up
 *   etc.
 */

#ifndef _VAST_INPUT_POINTER_RELEASE_H_
#define _VAST_INPUT_POINTER_RELEASE_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/pointer/PointerCode.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        class EventQueue;
        
        namespace pointer {
            class Release : public Component {
            private:
                
                hd::input::pointer::Control m_control;

                hd::input::pointer::Button m_button;


            private:

                friend class hd::input::EventQueue;

                hd::input::pointer::Control GetControl() const;

                hd::input::pointer::Button GetButton() const;


            protected:

                friend class ComponentPool<Release>;

                Release(unsigned int ID);


            public:
                
                void SetControl(hd::input::pointer::Control control);
                
                void SetButton(hd::input::pointer::Button button);
                
                void PrintDebugInfo() const;
                
                
            };
        }
    }
}

#endif