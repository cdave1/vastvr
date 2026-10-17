#include "Timer.h"
#include <Vast/VastMath.h>
#include "core/device/Device.h"
#ifdef WIN32
#include <time.h>
#include <chrono>
#else
#include <sys/time.h>
#endif

#if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
#include <QuartzCore/CABase.h>
#endif

using namespace hd::application;

Timer::Timer() :
    m_LastTimeInterval(0),
    m_TimeSinceLastFrame(0),
    m_startTime(0),
    m_LastFrameTime(0),
    m_currentTime(0),
    m_LastFPSUpdateTime(0),
    m_frames(0),
    m_totalFrames(0),
    m_totalSecs(0),
    m_currentFrame(0),
    m_isPrintable(true) {
        m_startTime = hd::Device::GetMediaTime();
}


const char * Timer::GetName() const {
    return "Timer";
}


void Timer::Destroy() {}


void Timer::Update() {
#ifdef WIN32
	m_currentTime = hd::Device::GetMediaTime();
#else
	#if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
		m_currentTime = CACurrentMediaTime();
	#else
		m_currentTime = hd::Device::GetMediaTime();
	#endif
#endif
	if (m_LastFPSUpdateTime == 0) {
		m_LastFPSUpdateTime = m_currentTime;
    }
	
	if (m_LastFrameTime == 0) {
		m_LastFrameTime = m_currentTime;
    }
    
	if ((m_currentTime - m_LastFPSUpdateTime) > 1.0f) {
		if (m_isPrintable) {
			hd::log::i("fps: %d\n", m_frames);
			m_totalFrames += m_frames;
			++m_totalSecs;
			if (m_totalSecs == 10) {
				hd::log::i("Frame rate: %d\n", m_totalFrames/m_totalSecs);
				m_totalSecs =0;
				m_totalFrames = 0;
			}
		}
		
		m_frames = 0;
		m_LastFPSUpdateTime = m_currentTime;
	}
    m_frames++;
    m_currentFrame++;
	
	m_TimeSinceLastFrame = m_currentTime - m_LastFrameTime;
}


hdTimestamp Timer::GetCurrentTime() const {
    return m_currentTime;
}


hdTimestamp Timer::GetElapsedTime() const {
    return m_currentTime - m_startTime;
}


uint32_t Timer::GetCurrentFrame() const {
    return m_currentFrame;
}
