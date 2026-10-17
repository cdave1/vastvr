#ifndef _VAST_INPUT_DEVICE_KEYBOARD_RESPONDER_H_
#define _VAST_INPUT_DEVICE_KEYBOARD_RESPONDER_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
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
        namespace device {
            class KeyboardResponder : public Component {
            private:

                int m_pointerId;

                std::function<void()> m_onKeyboard;


            protected:

                friend class ComponentPool<KeyboardResponder>;

                KeyboardResponder(unsigned int ID);


            public:

                void SetPointerId(unsigned int pointerId);

                int GetPointerId() const;

                template<class T>
                void OnSelect(T *src, void (T::*onSelect)(Entity *entity, const Vec3 &position));

                void Select(Entity *entity, const Vec3 &position);

            };
        }
    }
}


template<class T>
void hd::input::pointer::SelectResponder::OnSelect(T *src, void (T::*onSelect)(Entity *entity, const Vec3 &position)) {
    m_onSelect = std::bind(onSelect, src, std::placeholders::_1, std::placeholders::_2);
}

#endif