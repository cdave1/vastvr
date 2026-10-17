#include "core/application/Client.h"
#include "Browser.h"

static AppController *controller = nullptr;
static Browser *browser = nullptr;

using namespace hd::application;
void Client::Init() {
    controller = new AppController(this);
    browser = RegisterBrowserApps(controller, this);
}
void Client::WillShutdown() {}
void Client::Shutdown() {}
void Client::Pause() {}
void Client::Resume() {}
void Client::Update() { UpdateEventQueue(); controller->Update(); UpdateTimer(); }
void Client::Render() { controller->Draw(); browser->DrawCursor(Cameras::mainCamera); }
