#ifndef _VAST_MESSAGE_SCHEDULER_H_
#define _VAST_MESSAGE_SCHEDULER_H_

#include <list>

#include "TimedMessenger.h"


#define MessageSchedulerEventPoolSize 2048

namespace hd {
    class TimedMessenger;
    
    class MessageScheduler {
    private:
        
        bool IsLocked();
        
        std::list<hd::TimedMessenger *> m_currentReceivers;
        
        pthread_mutex_t m_mutex;
        
        
    public:
        static hd::MessageScheduler * DefaultEventScheduler();
        
        static void
        ScheduleReceiver(hd::TimedMessenger *handler, const float delay);
        
        static void StopReceiver(hd::TimedMessenger *event);
        
        /**
         * Start the event scheduler for the particular event.
         */
        void StartReceiver(hd::TimedMessenger *event);
        
        /**
         * Should be called once per frame from the main thread.
         */
        void Tick(const hdTimeInterval interval);
        
        
    protected:
        
        void StopReceiverInternal(hd::TimedMessenger *event);
        
        MessageScheduler();
        
        ~MessageScheduler();
        
    };
}


#endif