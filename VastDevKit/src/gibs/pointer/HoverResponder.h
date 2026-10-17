#ifndef _VAST_INPUT_POINTER_HOVER_RESPONDER_H_
#define _VAST_INPUT_POINTER_HOVER_RESPONDER_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/pointer/PointerCode.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    class Entity;

    namespace input {
        namespace pointer {
            class HoverResponder : public Component {
            private:
                
                hd::input::pointer::Control m_pointerControl;
                
                std::function<void(hd::Entity *entity, const Vec3 &position)> m_onHover;
                
                
            protected:
                
                friend class ComponentPool<HoverResponder>;
                
                HoverResponder(unsigned int ID);
                
                
            public:

                void SetPointerControl(hd::input::pointer::Control control);

                hd::input::pointer::Control GetPointerControl() const;
                
                template<class T>
                void OnHover(T *src, void (T::*onSelect)(Entity *entity, const Vec3 &position));
                
                void Hover(Entity *entity, const Vec3 &position);
                
            };
        }
    }
}

template<class T>
void hd::input::pointer::HoverResponder::OnHover(T *src, void (T::*onHover)(Entity *entity, const Vec3 &position)) {
    m_onHover = std::bind(onHover, src, std::placeholders::_1, std::placeholders::_2);
}

#endif