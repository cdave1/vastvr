#include "TimedMessenger.h"

hd::TimedMessenger::TimedMessenger(const hdTimeInterval delay) {
    m_timer = 0.0f;
    m_delay = hd::max(0.0, delay);
    m_started = false;
}


void hd::TimedMessenger::Start() {
    if (m_started) return;
    m_started = true;
}


bool hd::TimedMessenger::IsRunning() const {
    return m_started;
}


void hd::TimedMessenger::Tick(const hdTimeInterval interval) {
    hdAssert(m_started);
    m_timer += hd::max(0.0, interval);
}


void hd::TimedMessenger::Finish() {
    hdAssert(IsFinished());
    m_timer = 0.0f;
    m_started = false;
}


void hd::TimedMessenger::Stop() {
    hdAssert(IsRunning());
    m_timer = 0.0f;
    m_started = false;
}


bool hd::TimedMessenger::IsFinished() const {
    return (m_delay - m_timer) <= 0.0;
}


hdTimeInterval hd::TimedMessenger::GetElapsed() const {
    return m_timer;
}


hdTimeInterval hd::TimedMessenger::GetTimeRemaining() const {
    return hd::max(0.0, m_delay - m_timer);
}


void hd::TimedMessenger::SetDelay(const float delay) {
    if (IsRunning()) return;
    m_delay = delay;
}
