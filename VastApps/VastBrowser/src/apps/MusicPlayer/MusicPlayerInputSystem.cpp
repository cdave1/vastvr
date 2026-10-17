#include "MusicPlayerInputSystem.h"

MusicPlayerInputSystem::MusicPlayerInputSystem(hd::EntityPool *entityPool,
                                               hd::application::Client *client) :
EntitySystem(entityPool),
m_client(client) {}


void MusicPlayerInputSystem::Update() {
    hdAssert(m_client);
    hdAssert(m_client->GetEventQueue());

    const hd::Vector<hd::Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();

    for (int i = 0; i < events->Size(); ++i) {
        hd::Entity *event = events->At(i);

        auto baseEvent = event->Edit<hd::input::BaseEvent>();
        hdAssert(baseEvent);
        hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
        if (baseEvent->IsConsumed()) {
            continue;
        }

        if (event->ContainsComponent<hd::input::device::Keyboard>()) {
            KeyboardQuery(event);
        }
    }
}


void MusicPlayerInputSystem::KeyboardQuery(hd::Entity *event) {
    /*
    if (event->ContainsComponent<hd::input::button::Down>()) {
        auto down = event->Edit<hd::input::button::Down>();
        if (down->GetKeyCode() == hd::Device::Keycode::SPACE) {
            appMenu->ShowMenu(m_cameras.mainCamera);
        }
    }

    showMenu = false;
    if (event->ContainsComponent<device::Keyboard>()) {
        auto keyboard = event->Edit<device::Keyboard>();
        showMenu = keyboard->IsKeyDown(hd::Device::Keycode::SPACE);
    }*/
}