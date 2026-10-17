#ifndef _VAST_INPUT_POINTER_RELEASE_RESPONDER_H_
#define _VAST_INPUT_POINTER_RELEASE_RESPONDER_H_

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
            class ReleaseResponder : public Component {
            private:
                
                hd::input::pointer::Control m_control;

                hd::input::pointer::Button m_button;
                
                std::function<void(hd::Entity *entity, const Collision3 &collision)> m_onRelease;
                
                std::function<void(hd::Entity *entity, const Collision3 &collision)> m_onReleaseInside;
                
                std::function<void(hd::Entity *entity, const Collision3 &collision)> m_onReleaseOutside;
                
                
            protected:
                
                friend class ComponentPool<ReleaseResponder>;
                
                ReleaseResponder(unsigned int ID);
                
                
            public:

                void SetButton(hd::input::pointer::Button button);

                void SetControl(hd::input::pointer::Control control);

                hd::input::pointer::Control GetControl() const;

                hd::input::pointer::Button GetButton() const;

                void OnRelease(const std::function<void(hd::Entity *entity, const Collision3 &collision)> &callback);

                void OnReleaseInside(const std::function<void(hd::Entity *entity, const Collision3 &collision)> &callback);

                void OnReleaseOutside(const std::function<void(hd::Entity *entity, const Collision3 &collision)> &callback);
                
                template<class T>
                void OnRelease(T *src, void (T::*onSelect)(Entity *entity, const Collision3 &collision));
                
                template<class T>
                void OnReleaseInside(T *src, void (T::*onSelect)(Entity *entity, const Collision3 &collision));
                
                template<class T>
                void OnReleaseOutside(T *src, void (T::*onSelect)(Entity *entity, const Collision3 &collision));
                
                void Release(Entity *entity, const Collision3 &collision);
                
                void ReleaseInside(Entity *entity, const Collision3 &collision);
                
                void ReleaseOutside(Entity *entity, const Collision3 &collision);
                
            };
        }
    }
}


template<class T>
void hd::input::pointer::ReleaseResponder::OnRelease(T *src, void (T::*onRelease)(Entity *entity, const Collision3 &collision)) {
    m_onRelease = std::bind(onRelease, src, std::placeholders::_1, std::placeholders::_2);
}


template<class T>
void hd::input::pointer::ReleaseResponder::OnReleaseInside(T *src, void (T::*onReleaseInside)(Entity *entity, const Collision3 &collision)) {
    m_onReleaseInside = std::bind(onReleaseInside, src, std::placeholders::_1, std::placeholders::_2);
}


template<class T>
void hd::input::pointer::ReleaseResponder::OnReleaseOutside(T *src, void (T::*onReleaseOutside)(Entity *entity, const Collision3 &collision)) {
    m_onReleaseOutside = std::bind(onReleaseOutside, src, std::placeholders::_1, std::placeholders::_2);
}

#endif