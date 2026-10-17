// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#ifndef VAST_BROWSER_H
#define VAST_BROWSER_H
#include "common/AppPrototype.h"
#include "common/AppController.h"

class Browser : public AppPrototype {
public:
    Browser(hd::application::Client *client, AppController *controller);
    void AddDemo(const std::string &name);
    void OnActivate(hd::Entity *camera) override;
    void DrawCursor(hd::Entity *camera);
private:
    AppController *m_controller;
    unsigned int m_demoCount;
    hd::Entity *m_cursor;
};

Browser * RegisterBrowserApps(AppController *controller, hd::application::Client *client);
#endif
