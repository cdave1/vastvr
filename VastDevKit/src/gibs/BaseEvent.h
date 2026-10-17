#ifndef _VAST_INPUT_BASE_EVENT_H_
#define _VAST_INPUT_BASE_EVENT_H_

/**
 * All events will have this component.
 */

#include "core/component/Component.h"
#include "core/entity/Entity.h"

template<class T> class ComponentPool;

namespace hd
{
    namespace input
    {
        class BaseEvent : public Component
        {
        private:
            
            hdTimestamp m_timestamp;
            
            unsigned int m_frame;
            
            EntityIdentifier m_consumer;
            
            
        protected:
            
            friend class ComponentPool<BaseEvent>;
            friend class EventQueue;
            
            BaseEvent(unsigned int ID);
            
            void SetTimestamp(hdTimestamp timestamp);
            
            void SetFrame(unsigned int frame);
            
            
        public:
            
            void Reset();
            
            hdTimeInterval GetTimestamp() const;
            
            unsigned int GetFrame() const;
            
            EntityIdentifier GetConsumer() const;
            
            bool IsConsumed() const;
            
            void ConsumeEvent(EntityIdentifier consumer);
            
            void PrintDebugInfo() const;
            
        };
    }
}

#endif