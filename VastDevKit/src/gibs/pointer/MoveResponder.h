#ifndef _VAST_INPUT_POINTER_MOVE_RESPONDER_H_
#define _VAST_INPUT_POINTER_MOVE_RESPONDER_H_

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
            class MoveResponder : public Component {
            private:
                
                hd::input::pointer::Control m_control;
                
                std::function<void(hd::Entity *entity, const Collision3 &collision, const Vec3 &delta)> m_onMove;
                
                
            protected:
                
                friend class ComponentPool<MoveResponder>;
                
                MoveResponder(unsigned int ID);
                
                
            public:

                void SetControl(hd::input::pointer::Control control);

                hd::input::pointer::Control GetControl() const;
                
                template<class T>
                void OnMove(T *src, void (T::*onSelect)(Entity *entity, const Collision3 &collision, const Vec3 &delta));
                
                void Move(Entity *entity, const Collision3 &position, const Vec3 &delta);
                
            };
        }
    }
}

template<class T>
void hd::input::pointer::MoveResponder::OnMove(T *src, void (T::*onMove)(Entity *entity, const Collision3 &collision, const Vec3 &delta)) {
    m_onMove = std::bind(onMove, src, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

#endif