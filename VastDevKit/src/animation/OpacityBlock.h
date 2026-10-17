#ifndef _VAST_OPACITY_MODIFIER_H_
#define _VAST_OPACITY_MODIFIER_H_

#include "core/component/Component.h"

template<class T> class ComponentPool;

///
/// Lerps opacity between two values.
///
namespace hd {
    namespace animation {
        class OpacityBlock : public Component {
        private:

            float m_startOpacity;

            float m_endOpacity;

            bool m_isEnabled;


        protected:

            friend class ComponentPool<OpacityBlock>;

            OpacityBlock(unsigned int ID);


        public:

            void Reset();

            void Reverse();

            void Enable();

            void Disable();

            bool IsEnabled() const;

            void SetStartOpacity(const float opacity);

            void SetEndOpacity(const float opacity);

            float GetOpacity(const float timer) const;

        };
    }
}

#endif