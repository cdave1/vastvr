#ifndef _VAST_SCALE_BLOCK_H_
#define _VAST_SCALE_BLOCK_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

template<class T> class ComponentPool;

///
/// Lerps between two points.
///
namespace hd {
    namespace animation {
        class ScaleBlock : public Component {
        private:

            Vec3 m_startScale;

            Vec3 m_endScale;

            bool m_isEnabled;


        protected:

            friend class ComponentPool<ScaleBlock>;

            ScaleBlock(unsigned int ID);


        public:

            void Reset();

            void Reverse();

            void Enable();

            void Disable();

            bool IsEnabled() const;

            void SetStartScale(const Vec3 &scale);

            void SetEndScale(const Vec3 &scale);

            Vec3 GetScale(const float timer) const;

        };
    }
}

#endif