// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#include "core/application/Client.h"
#include "common/AppController.h"
#include "PhysicsSandbox.h"

static AppController *controller = nullptr;
static PhysicsSandbox *sandbox = nullptr;
using namespace hd::application;
void Client::Init() {
    controller = new AppController(this);
    sandbox = new PhysicsSandbox(this);
    controller->AddApp(sandbox); controller->SelectApp("PhysicsHotel");
}
void Client::WillShutdown() { if (sandbox) sandbox->OnDeactivate(); }
void Client::Shutdown() {}
void Client::Pause() { if (sandbox) sandbox->OnDeactivate(); }
void Client::Resume() {}
void Client::Update() { UpdateEventQueue(); sandbox->ProcessInput(); controller->Update(); UpdateTimer(); }
void Client::Render() { controller->Draw(); }
