#ifndef _VAST_POSITION_MODIFIER_H_
#define _VAST_POSITION_MODIFIER_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

template<class T> class ComponentPool;

///
/// Lerps between two points.
///
namespace hd {
    namespace animation {
        class PositionBlock : public Component {
        private:

            Vec3 m_startPosition;

            Vec3 m_endPostion;

            bool m_isEnabled;


        protected:

            friend class ComponentPool<PositionBlock>;

            PositionBlock(unsigned int ID);


        public:

            void Reset();

            void Reverse();

            void Enable();

            void Disable();

            bool IsEnabled() const;

            void SetStartPosition(const Vec3 &position);

            const Vec3 & GetStartPosition() const { return m_startPosition; }

            void SetEndPosition(const Vec3 &position);

            Vec3 GetPosition(const float timer) const;

        };
    }
}

#endif