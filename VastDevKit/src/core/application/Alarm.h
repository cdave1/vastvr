#ifndef _VAST_CORE_APPLICATION_ALARM_H_
#define _VAST_CORE_APPLICATION_ALARM_H_

#include "core/component/Component.h"

template<class T> class ComponentPool;

namespace hd {
    namespace application {
        class Alarm : public Component {
        private:
            
            float m_duration;

            float m_elapsed;

            bool m_isActive;

            bool m_loops;

            std::function<void(const hd::application::Alarm *alarm)> m_callback;


        protected:

            friend class ComponentPool<hd::application::Alarm>;

            Alarm(unsigned int ID);
            
            
        public:
            
            void Reset();

            void Start();

            void Stop();

            bool IsActive();

            void SetCallback(std::function<void(const hd::application::Alarm *alarm)> callback);

            void SetLoops(bool loops);

            float GetTime() const;

            void SetDuration(const float duration);
            
            void Update(const float elapsed);

        };
    }
}

#endif
