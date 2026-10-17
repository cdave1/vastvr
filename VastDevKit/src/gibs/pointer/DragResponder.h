#ifndef _VAST_INPUT_POINTER_DRAG_RESPONDER_H_
#define _VAST_INPUT_POINTER_DRAG_RESPONDER_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/pointer/PointerCode.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

//
// Entity required for callback signatures.
//
namespace hd {
    class Entity;
}


namespace hd {
    namespace input {
        namespace pointer {
            class DragResponder : public Component {
            private:
                
                hd::input::pointer::Control m_control;

                hd::input::pointer::Button m_button;
                
                std::function<void(hd::Entity *entity, const Collision3 &collision, const Vec3 &delta)> m_onDrag;
                
                
            protected:
                
                friend class ComponentPool<DragResponder>;
                
                DragResponder(unsigned int ID);
                
                
            public:

                void SetButton(hd::input::pointer::Button button);

                void SetControl(hd::input::pointer::Control control);

                hd::input::pointer::Control GetControl() const;
                
                hd::input::pointer::Button GetButton() const;
                
                template<class T>
                void OnDrag(T *src, void (T::*onSelect)(Entity *entity, const Collision3 &collision, const Vec3 &delta));
                
                void Drag(Entity *entity, const Collision3 &position, const Vec3 &delta);
                
            };
        }
    }
}

template<class T>
void hd::input::pointer::DragResponder::OnDrag(T *src, void (T::*onDrag)(Entity *entity, const Collision3 &collision, const Vec3 &delta)) {
    m_onDrag = std::bind(onDrag, src, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

#endif