#ifndef _VAST_CORE_APPLICATION_TIMER_H_
#define _VAST_CORE_APPLICATION_TIMER_H_

namespace hd {
    namespace application {
        class Timer {
        private:
            
            //
            // Time intervals
            //
            hdTimeInterval	m_LastTimeInterval;
            
            hdTimeInterval	m_TimeSinceLastFrame;
            
            //
            // Times
            //
            hdTimestamp m_startTime;
            
            hdTimestamp	m_LastFrameTime;
            
            hdTimestamp	m_currentTime;
            
            hdTimestamp	m_LastFPSUpdateTime;
            
            //
            // Frame counters
            //
            uint32_t m_frames;
            
            uint32_t m_totalFrames;
            
            uint32_t m_totalSecs;
            
            uint32_t m_currentFrame;
            
            bool m_isPrintable;
            
            
        public:
            
            Timer();
            
            const char * GetName() const;
            
            void Destroy();
            
            void Update();
            
            hdTimestamp GetCurrentTime() const;
            
            hdTimestamp GetElapsedTime() const;
            
            uint32_t GetCurrentFrame() const;
            
        };
    }
}

#endif
