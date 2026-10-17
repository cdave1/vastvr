// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#include "MusicPlayer.h"
#include "LocalAudio.h"
using namespace hd;
using namespace hd::graphics;
using namespace hd::input;

MusicPlayer::MusicPlayer(hd::application::Client *client) : AppPrototype(client), m_effect(0) {
    ConstructSkybox(3);
    const char *labels[] = {"Choose audio", "Project sound", "Stop"};
    for (int i = 0; i < 3; ++i) {
        const Vec3 position((i - 1) * 440.0f, 150, 0);
        auto button = AddCube(360, 160, 32, position);
        button->Add<ui::Identifier>()->SetId(labels[i]);
        AddLabel(position + Vec3(0, 0, 20), Vec3(), labels[i]);
        button->Edit<pointer::ReleaseResponder>()->OnReleaseInside([this, i](const Entity *, const Collision3 &) {
            if (i == 0) {
                const std::string path = ChooseLocalAudioFile();
                if (!path.empty()) PlayAudio(path);
            } else if (i == 1) {
                PlayAudio(FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav"));
            } else StopAudio();
        });
    }
    m_status = AddLabel(Vec3(0, 400, 20), Vec3(), "No music bundled. Choose a local audio file.");
    AddLabel(Vec3(0, -100, 20), Vec3(), "Audio loops until Stop. Tab returns to the browser.");
}

void MusicPlayer::SetStatus(const std::string &value) {
    m_status->Edit<ui::Text>()->SetText(value);
    auto font = m_status->Edit<ui::Font>();
    auto mesh = m_status->Edit<ui::FontMesh>();
    mesh->RenderMesh(font, value);
    auto buffer = m_status->Edit<VertexBuffer>();
    buffer->AddMeshes(mesh->GetMeshData());
    buffer->SetDefaultAttributeLocations(m_status->Get<Shader>());
    m_status->Edit<Transform>()->SetOrigin(Vec3(0, 400, 20) - font->GetSize(value).Half());
}

bool MusicPlayer::PlayAudio(const std::string &path) {
    unsigned int effect = sound::SoundController::PlayEffectWithLoop(path.c_str());
    if (!effect) { SetStatus("Could not play that audio file."); return false; }
    sound::SoundController::StopEffect(m_effect);
    m_effect = effect;
    SetStatus("Playing: " + path.substr(path.find_last_of('/') + 1));
    return true;
}

void MusicPlayer::StopAudio() {
    sound::SoundController::StopEffect(m_effect);
    m_effect = 0;
    SetStatus("Stopped. Choose a local audio file or project sound.");
}
void MusicPlayer::OnDeactivate() { StopAudio(); AppPrototype::OnDeactivate(); }
