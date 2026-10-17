#include "core/application/Client.h"

#include <Vast/Vast.h>
#include "common/AppController.h"

#include "Builder.h"

static AppController *m_appController = NULL;


using namespace hd::application;

void Client::Init() {
    m_appController = new AppController(this);

    auto builder = new Builder(this);
    builder->m_name = "Builder";
    builder->m_iconPath = "resources/images/paint.png";
    m_appController->AddApp(builder);

    
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
