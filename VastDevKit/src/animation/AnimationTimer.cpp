#include "AnimationTimer.h"
#include "math/Geometry.h"
#include "graphics/gl/GraphicsInterface.h"

hd::animation::AnimationTimer::AnimationTimer(unsigned int ID) : Component(ID),
    m_duration(0),
    m_delta(0),
    m_elapsed(0),
    m_isActive(false),
    m_timingFunction(TimingFunction::SmoothStep) {
    Reset();
}


void hd::animation::AnimationTimer::Reset() {
    m_duration = 0.0f;
    m_delta = 0.0f;
    m_elapsed = 0.0f;
    m_isActive = false;
    m_timingFunction = TimingFunction::SmoothStep;
}


void hd::animation::AnimationTimer::Start() {
    if (m_duration <= 0.0f) {
        hd::log::e("Can't start a timer with a negative or zero duration\n");
        return;
    }
    m_elapsed = 0.0f;
    m_isActive = true;
}


void hd::animation::AnimationTimer::Stop() {
    m_isActive = false;
}


bool hd::animation::AnimationTimer::IsActive() const {
    return m_isActive;
}


float hd::animation::AnimationTimer::GetTime() const {
    float time = m_elapsed / m_duration;
    switch (m_timingFunction) {
        case TimingFunction::Linear:
            break;
        case TimingFunction::BounceEaseIn:
            break;
        case TimingFunction::BounceEaseOut:
            break;
        case TimingFunction::BounceEaseInOut:
            break;
        case TimingFunction::SmoothStep:
        default:
            time = hd::smoothstep(0.1f, 0.9f, m_elapsed / m_duration);
    }
    return time;
}


void hd::animation::AnimationTimer::SetDuration(const float duration) {
    if (m_isActive) {
        hd::log::e("Can't change duration of a running animation timer.\n");
        return;
    }
    m_duration = duration;
}


void hd::animation::AnimationTimer::Update(const float delta) {
    if (m_isActive == false) {
        return;
    }

    m_delta = delta;
    m_elapsed += delta;
    if (m_elapsed > m_duration) {
        m_isActive = false;
        m_elapsed = m_duration;
    }
}
