#include "Alarm.h"
#include <Vast/VastMath.h>

hd::application::Alarm::Alarm(unsigned int ID) : Component(ID),
m_duration(0),
m_elapsed(0),
m_isActive(false),
m_loops(false),
m_callback() {
    Reset();
}


void hd::application::Alarm::Reset() {
    m_duration = 0.0f;
    m_elapsed = 0.0f;
    m_isActive = false;
    m_loops = false;
    m_callback = nullptr;
}


void hd::application::Alarm::Start() {
    if (m_duration <= 0.0f) {
        hd::log::e("Can't start alarm with a negative or zero duration");
        return;
    }
    m_elapsed = 0.0f;
    m_isActive = true;
}


void hd::application::Alarm::Stop() {
    m_isActive = false;
}


bool hd::application::Alarm::IsActive() {
    return m_isActive;
}


void hd::application::Alarm::SetCallback(std::function<void(const hd::application::Alarm *alarm)> callback) {
    m_callback = callback;
}


void hd::application::Alarm::SetLoops(bool loops) {
    m_loops = loops;
}


void hd::application::Alarm::SetDuration(const float duration) {
    if (m_isActive) {
        hd::log::e("Can't change duration of alarm.");
        return;
    }
    m_duration = duration;
}


void hd::application::Alarm::Update(const float elapsed) {
    if (m_isActive == false) {
        return;
    }

    m_elapsed += elapsed;
    if (m_elapsed > m_duration) {
        m_isActive = false;
        m_elapsed = m_duration;

        if (m_callback) {
            m_callback(this);
        }

        if (m_loops) {
            m_elapsed = 0.0f;
            m_isActive = true;
        }
    }
}
