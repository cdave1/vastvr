#ifndef _VAST_CORE_APPLICATION_CLIENT_H_
#define _VAST_CORE_APPLICATION_CLIENT_H_

namespace hd {
    namespace input {
        class EventQueue;
    }
}

namespace hd {
    namespace application {
        class Timer;
        
        class Client {
        public:
            
            Client();
            
            ~Client();
            
            void Init();
            
            void WillShutdown();
            
            void Shutdown();
            
            void Pause();
            
            void Resume();
            
            hd::input::EventQueue * GetEventQueue();
            
            void Update();
            
            void Render();
            
            const hd::application::Timer * GetTimer() const;
            
            
        private:
            
            void UpdateEventQueue();
            
            void UpdateTimer();
            
            hd::input::EventQueue *m_eventQueue;
            
            hd::application::Timer *m_timer;
            
        };
    }
}

#endif