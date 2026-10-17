#ifndef _VAST_ANIMATION_TIMER_H_
#define _VAST_ANIMATION_TIMER_H_

#include "core/component/Component.h"
#include <Vast/VastMath.h>

template<class T> class ComponentPool;

///
/// Lerps a linear timer between 0.0f and 1.0f using a specific timing function.
///
namespace hd {
    namespace animation {

        enum class TimingFunction {
            Linear,
            SmoothStep,
            QuadEaseIn,
            QuadEaseOut,
            QuadEaseInOut,
            ElasticEaseIn,
            ElasticEaseOut,
            ElasticEaseInOut,
            BounceEaseIn,
            BounceEaseOut,
            BounceEaseInOut
        };

        class AnimationTimer : public Component {
        private:

            float m_duration;

            float m_elapsed;

            float m_delta;

            bool m_isActive;

            TimingFunction m_timingFunction;


        protected:

            friend class ComponentPool<AnimationTimer>;

            AnimationTimer(unsigned int ID);


        public:

            void Reset();

            void Start();

            void Stop();

            bool IsActive() const;

            float GetTime() const;

            void SetDuration(const float duration);

            void Update(const float delta);

        };
    }
}

#endif