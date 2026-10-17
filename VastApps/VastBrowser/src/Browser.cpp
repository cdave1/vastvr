// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#include "Browser.h"
#include "apps/MusicPlayer/MusicPlayer.h"
#include "apps/Piano/Piano.h"
#include "apps/SSAOTest/SSAOTest.h"
#include "apps/ShadowTest/ShadowTest.h"
#include "apps/TeapotDemo/TeapotDemo.h"

using namespace hd;
using namespace hd::graphics;
using namespace hd::input;

Browser::Browser(hd::application::Client *client, AppController *controller) :
    AppPrototype(client), m_controller(controller), m_demoCount(0) {
    ConstructSkybox(3);
    auto title = AddLabel(Vec3(0, 520, 20), Vec3(), "Vast Browser");
    title->Edit<Transform>()->SetScale(1.6f);
    AddLabel(Vec3(0, -350, 20), Vec3(), "Arrow keys: look   Space: select   Tab: home   1 / 2: previous / next");
    m_cursor = AddLabel(Vec3(), Vec3(), "+");
    m_cursor->SetName("GazeCursor");
}

void Browser::AddDemo(const std::string &name) {
    const float x = (int(m_demoCount % 3) - 1) * 440.0f;
    const float y = 220.0f - float(m_demoCount / 3) * 270.0f;
    auto button = AddCube(400, 180, 32, Vec3(x, y, 0));
    button->Add<ui::Identifier>()->SetId(name);
    button->Edit<Material>()->SetAmbient(Color4f(0.12f, 0.24f, 0.38f, 1.0f));
    button->Edit<pointer::ReleaseResponder>()->OnReleaseInside([this, name](const Entity *, const Collision3 &) {
        m_controller->SelectApp(name);
        sound::SoundController::PlayEffect(FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav").c_str());
    });
    AddLabel(Vec3(x, y, 20), Vec3(), name);
    ++m_demoCount;
}

void Browser::OnActivate(hd::Entity *camera) {
    auto headset = camera->Edit<Headset>();
    headset->SetPosition(Vec3(0, 300, 1500));
    headset->SetOrientationOffset(Quaternion::FromTwoVectors(Vec3(0, 0, -1), Vec3(0, -0.1f, -1)));
}

void Browser::DrawCursor(hd::Entity *camera) {
    auto headset = camera->Get<Headset>();
    auto transform = m_cursor->Edit<Transform>();
    const Quaternion rotation = headset->GetWorldOrientation();
    const AABB bounds = m_cursor->Edit<ui::Font>()->GetSize("+");
    transform->SetScale(0.05f);
    transform->SetOrigin(headset->GetWorldPosition() + rotation * (Vec3(0, 0, -60) - bounds.Half() * 0.05f));
    transform->SetRotation(rotation);
    transform->Update();
    GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    auto shader = m_cursor->Edit<Shader>();
    shader->Begin();
    shader->SetUniform4f(MATERIAL_AMBIENT_COLOR_UNIFORM, Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    shader->SetUniform1f(OPACITY_VALUE_UNIFORM, 1.0f);
    shader->SetUniformMatrix(WORLD_MATRIX_UNIFORM, transform->GetMatrix());
    shader->SetUniformMatrix(VIEW_MATRIX_UNIFORM, headset->GetLookAtMatrix());
    shader->SetUniformMatrix(PROJECTION_MATRIX_UNIFORM, headset->GetProjection());
    m_cursor->Get<VertexBuffer>()->Render();
    shader->End();
    if (depth) glEnable(GL_DEPTH_TEST);
}

Browser * RegisterBrowserApps(AppController *controller, hd::application::Client *client) {
    auto browser = new Browser(client, controller);
    browser->m_name = "Browser";
    controller->AddApp(browser);
    const std::pair<AppPrototype *, std::string> demos[] = {
        {new TeapotDemo(client), "Teapots"},
        {new ShadowTest(client), "Shadows"},
        {new SSAOTest(client), "Geometry"},
        {new Piano(client), "Piano"},
        {new MusicPlayer(client), "Music & audio"},
    };
    for (const auto &demo : demos) {
        demo.first->m_name = demo.second;
        controller->AddApp(demo.first);
        browser->AddDemo(demo.second);
    }
    controller->SetHomeApp(browser->m_name);
    controller->SelectApp(browser->m_name);
    return browser;
}
