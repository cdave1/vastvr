#include "core/application/Client.h"

#include <Vast/Vast.h>
#include "common/AppController.h"

#include "Game.h"

static AppController *m_appController = NULL;


using namespace hd::application;

void Client::Init() {
    m_appController = new AppController(this);

    auto game = new Game(this);
    game->m_name = "Builder";
    game->m_iconPath = "resources/images/paint.png";
    m_appController->AddApp(game);

    
}


void Client::WillShutdown() {
}


void Client::Shutdown() {
}


void Client::Pause() {
}


void Client::Resume() {
}


void Client::Update() {
    // Pulls list of entities of event queue and sends events to various
    // sub systems.
    
    UpdateEventQueue();
    
    m_appController->Update();
    
    UpdateTimer();
}


void Client::Render() {
    m_appController->Draw();
    //GetEventQueue()->DebugRender();
}
