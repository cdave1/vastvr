#ifndef _VAST_INPUT_LOOKAT_RESPONDER_H_
#define _VAST_INPUT_LOOKAT_RESPONDER_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    class Entity;

    namespace input {
        class Headset;

        class LookAtResponder : public Component {
        private:

            std::function<void(const hd::Entity *entity, const hd::input::Headset *headset, const Collision3 &collision)> m_onLookAt;

            std::function<void(hd::Entity *entity, hd::input::Headset *headset, const Collision3 &collision)> m_onLookAtBegin;

            std::function<void(hd::Entity *entity, hd::input::Headset *headset, const Collision3 &collision)> m_onLookAtHover;

            std::function<void(hd::Entity *entity, hd::input::Headset *headset, const Collision3 &collision)> m_onLookAtEnd;

            int m_pointerId;

            bool m_isEnabled;


        protected:

            friend class ComponentPool<LookAtResponder>;

            LookAtResponder(unsigned int ID);


        public:

            void SetPointerId(unsigned int pointerId);

            int GetPointerId() const;

            void Enable();

            void Disable();

            bool IsEnabled() const;

            void OnLookAt(const std::function<void(const hd::Entity *entity, const hd::input::Headset *headset, const Collision3 &collision)> &callback);

            void LookAt(const Entity *entity, const hd::input::Headset *headset, const Collision3 &collision);

            template<class T>
            void OnLookAtBegin(T *src, void (T::*onSelect)(Entity *entity, hd::input::Headset *headset, const Collision3 &collision));

            void LookAtBegin(Entity *entity, hd::input::Headset *headset, const Collision3 &collision);

            template<class T>
            void OnLookAtHover(T *src, void (T::*onSelect)(Entity *entity, hd::input::Headset *headset, const Collision3 &collision));

            void LookAtHover(Entity *entity, hd::input::Headset *headset, const Collision3 &collision);

            template<class T>
            void OnLookAtEnd(T *src, void (T::*onSelect)(Entity *entity, hd::input::Headset *headset, const Collision3 &collision));

            void LookAtEnd(Entity *entity, hd::input::Headset *headset, const Collision3 &collision);

        };
    }
}


template<class T>
void hd::input::LookAtResponder::OnLookAtBegin(T *src, void (T::*onLookAtBegin)(Entity *entity, hd::input::Headset *headset, const Collision3 &collision)) {
    m_onLookAtBegin = std::bind(onLookAtBegin, src, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}


template<class T>
void hd::input::LookAtResponder::OnLookAtHover(T *src, void (T::*onLookAtHover)(Entity *entity, hd::input::Headset *headset, const Collision3 &collision)) {
    m_onLookAtHover = std::bind(onLookAtHover, src, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}


template<class T>
void hd::input::LookAtResponder::OnLookAtEnd(T *src, void (T::*onLookAtEnd)(Entity *entity, hd::input::Headset *headset, const Collision3 &collision)) {
    m_onLookAtEnd = std::bind(onLookAtEnd, src, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

#endif