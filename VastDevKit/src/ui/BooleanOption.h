#ifndef _VAST_UI_BOOLEAN_OPTION_H_
#define _VAST_UI_BOOLEAN_OPTION_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

template<class T> class ComponentPool;

namespace hd {
    class Entity;

    namespace ui {

        class BooleanOption : public Component {
        private:

            bool m_currentState;

            std::function<void(bool state, hd::Entity *sender)> m_onOptionChanged;

        protected:
            
            friend class ComponentPool<BooleanOption>;
            
            BooleanOption(unsigned int ID);
            
        public:

            void On(hd::Entity *sender);

            void Off(hd::Entity *sender);

            bool IsOn() const {
                return m_currentState == true;
            }

            void OnOptionChanged(const std::function<void(bool state, hd::Entity *sender)> &callback);
            
        };
    }
}

#endif