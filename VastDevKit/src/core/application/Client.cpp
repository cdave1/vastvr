#include "core/application/Client.h"
#include "Vast/VastGibs.h"
#include "core/application/Timer.h"
#include "SDL2/SDL.h"

using namespace hd::application;
using namespace hd::input;

Client::Client() :
    m_timer(NULL),
    m_eventQueue(NULL) {
        if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) < 0) {
            hd::log::e("Failed to init SDL\n");
            exit(1);
        }
        
        m_timer = new Timer();
        m_eventQueue = new EventQueue(m_timer);
        m_eventQueue->DiscoverInputDevices();
}


Client::~Client() {}


hd::input::EventQueue * Client::GetEventQueue() {
    return m_eventQueue;
}


const hd::application::Timer * Client::GetTimer() const {
    return m_timer;
}


void Client::UpdateEventQueue() {
//    SDL_Delay(1);
    m_eventQueue->Update();
}


void Client::UpdateTimer() {
    m_timer->Update();
}
