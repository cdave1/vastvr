#ifndef _VAST_INPUT_POINTER_RESPONDER_STATE_H_
#define _VAST_INPUT_POINTER_RESPONDER_STATE_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    namespace input {
        namespace pointer {
            ///
            /// TODO: fix poorly named enum.
            ///
            enum class SelectionState {
                FREE,

                SELECTED,
                HELD,
                DRAGGED,
                RELEASED
            };

            enum class LookAtState {
                FREE,
                LOOKAT
            };
            
            class ResponderState : public Component {
            private:
                
                SelectionState m_selectionState;

                LookAtState m_lookAtState;
                
                
            protected:
                
                friend class ComponentPool<ResponderState>;
                
                ResponderState(unsigned int ID);
                
                
            public:
                
                SelectionState GetCurrentState() const { return m_selectionState; }

                LookAtState GetLookAtState() const { return m_lookAtState; }

                void Select();
                
                void Drag();
                
                void Release();

                void LookAt();

                void UnLookAt();
                
            };
        }
    }
}

#endif