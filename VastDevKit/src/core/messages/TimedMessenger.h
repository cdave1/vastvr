#ifndef _VAST_TIMED_MESSENGER_H_
#define _VAST_TIMED_MESSENGER_H_

#include <Vast/VastMath.h>

#define _VAST_TIMED_MESSENGER_NAME "TimedMessengerName"

/**
 * Counts down a duration, and then sends an event.
 */
namespace hd {
    class TimedMessenger {
    private:
        hdTimeInterval m_delay;
        
        hdTimeInterval m_timer;
        
        bool m_started;
        
    public:
        
        TimedMessenger(const hdTimeInterval delay);
        
        bool IsFinished() const;
        
        hdTimeInterval GetElapsed() const;
        
        hdTimeInterval GetTimeRemaining() const;
        
        void SetDelay(const float delay);
        
        
    private:
        
        friend class MessageScheduler;
        
        void Tick(const hdTimeInterval interval);
        
        void Start();
        
        void Finish();
        
        void Stop();
        
        bool IsRunning() const;
        
    };
}

#endif
