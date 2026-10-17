#include "BaseEvent.h"

using namespace hd::input;

BaseEvent::BaseEvent(unsigned int ID) : Component(ID),
    m_timestamp(0),
    m_frame(0),
    m_consumer(hd::NullEntityIdentifier) {
}


void BaseEvent::SetTimestamp(hdTimestamp timestamp) {
    m_timestamp = timestamp;
}


void BaseEvent::SetFrame(unsigned int frame) {
    m_frame = frame;
}


void BaseEvent::Reset() {
    m_timestamp = 0;
    m_frame = 0;
    m_consumer = hd::NullEntityIdentifier;
}


hdTimeInterval BaseEvent::GetTimestamp() const {
    return m_timestamp;
}


unsigned int BaseEvent::GetFrame() const {
    return m_frame;
}


EntityIdentifier BaseEvent::GetConsumer() const {
    return m_consumer;
}


bool BaseEvent::IsConsumed() const {
    return (m_consumer != hd::NullEntityIdentifier);
}


void BaseEvent::ConsumeEvent(EntityIdentifier consumer) {
    if (m_consumer == hd::NullEntityIdentifier) {
        m_consumer = consumer;
    }
}


void BaseEvent::PrintDebugInfo() const {
    Component::PrintDebugInfo();
    hd::log::i("\tBaseEvent: Timestamp: %f, Frame: %d\n", m_timestamp, m_frame);
}