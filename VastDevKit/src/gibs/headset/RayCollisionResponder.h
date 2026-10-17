#ifndef _VAST_INPUT_RAY_COLLISION_RESPONDER_H_
#define _VAST_INPUT_RAY_COLLISION_RESPONDER_H_

#include <Vast/VastMath.h>
#include "core/component/Component.h"
#include "gibs/Constants.h"

template<class T> class ComponentPool;

namespace hd {
    class Entity;

    namespace input {
        class Headset;

        class RayCollisionResponder : public Component {
        private:

            std::function<void(hd::Entity *entity, hd::input::Headset *headset, const Collision3 &collision)> m_onRayCollision;

            int m_pointerId;

            bool m_isEnabled;


        protected:

            friend class ComponentPool<RayCollisionResponder>;

            RayCollisionResponder(unsigned int ID);


        public:

            void SetPointerId(unsigned int pointerId);

            int GetPointerId() const;

            void Enable();

            void Disable();

            bool IsEnabled() const;

            void OnRayCollision(const std::function<void(hd::Entity *entity, hd::input::Headset *headset, const Collision3 &collision)> &callback);

            void RayCollision(Entity *entity, hd::input::Headset *headset, const Collision3 &collision);

        };
    }
}

#endif