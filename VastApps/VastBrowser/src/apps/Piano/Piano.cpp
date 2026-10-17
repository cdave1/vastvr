// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#include "Piano.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
using namespace hd;
using namespace hd::graphics;
using namespace hd::input;

Piano::Piano(hd::application::Client *client) :
    AppPrototype(client), m_audioDevice(0), m_sampleRate(48000), m_lastNote(-1) {
    ConstructSkybox(3);
    AddFloorCube(3200, 40, Vec3(0, -100, 0));
    AddCube(1720, 180, 320, Vec3(0, 90, -20));
    int white = 0;
    for (int note = 21; note <= 108; ++note) {
        const int pitch = note % 12;
        const bool black = pitch == 1 || pitch == 3 || pitch == 6 || pitch == 8 || pitch == 10;
        const float x = -816.0f + (black ? white - 0.5f : float(white)) * 32.0f;
        auto key = AddCube(black ? 20 : 28, 20, black ? 130 : 220,
                           Vec3(x, black ? 210 : 190, black ? -45 : 0));
        key->Add<ui::Identifier>()->SetId(std::to_string(note));
        key->Edit<Material>()->SetAmbient(black ? Color4f(0.03f, 0.03f, 0.04f, 1.0f) : Color4f(0.98f, 0.98f, 0.95f, 1.0f));
        key->Edit<pointer::SelectResponder>()->OnSelect(this, &Piano::OnKeySelect);
        key->Edit<pointer::ReleaseResponder>()->OnRelease(this, &Piano::OnKeyRelease);
        if (!black) ++white;
    }
    AddLabel(Vec3(0, 450, -100), Vec3(), "Look at a key and press Space. Tab returns to the browser.");
}

Piano::~Piano() { if (m_audioDevice) SDL_CloseAudioDevice(m_audioDevice); }
void Piano::OnActivate(hd::Entity *camera) {
    AppPrototype::OnActivate(camera);
    camera->Edit<Headset>()->SetPosition(Vec3(0, 700, 2100));
}
void Piano::OnDeactivate() {
    if (m_audioDevice) SDL_ClearQueuedAudio(m_audioDevice);
    AppPrototype::OnDeactivate();
}
unsigned int Piano::QueuedAudioBytes() const {
    return m_audioDevice ? SDL_GetQueuedAudioSize(m_audioDevice) : 0;
}
bool Piano::PlayNote(int note) {
    if (!m_audioDevice) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) return false;
        SDL_AudioSpec requested = {}, actual = {};
        requested.freq = m_sampleRate; requested.format = AUDIO_F32SYS;
        requested.channels = 1; requested.samples = 512;
        m_audioDevice = SDL_OpenAudioDevice(nullptr, 0, &requested, &actual, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
        if (!m_audioDevice) { log::e("Piano audio: %s\n", SDL_GetError()); return false; }
        m_sampleRate = actual.freq;
    }
    const double frequency = 440.0 * std::pow(2.0, (note - 69) / 12.0);
    std::vector<float> samples(size_t(m_sampleRate * 0.8));
    for (size_t i = 0; i < samples.size(); ++i) {
        const double t = double(i) / m_sampleRate;
        const double phase = 2.0 * 3.141592653589793 * frequency * t;
        const double envelope = std::min(t / 0.008, 1.0) * std::exp(-6.0 * t) * std::min((0.8 - t) / 0.02, 1.0);
        samples[i] = float(0.2 * envelope * (std::sin(phase) + 0.2 * std::sin(2 * phase)));
    }
    SDL_ClearQueuedAudio(m_audioDevice);
    if (SDL_QueueAudio(m_audioDevice, samples.data(), Uint32(samples.size() * sizeof(float))) != 0) return false;
    SDL_PauseAudioDevice(m_audioDevice, 0);
    m_lastNote = note;
    return true;
}
void Piano::OnKeySelect(hd::Entity *element, const Collision3 &) {
    auto transform = element->Edit<Transform>();
    transform->SetOrigin(transform->GetOrigin() - Vec3(0, 10, 0));
    PlayNote(std::stoi(element->Get<ui::Identifier>()->GetId()));
}
void Piano::OnKeyRelease(hd::Entity *element, const Collision3 &) {
    auto transform = element->Edit<Transform>();
    transform->SetOrigin(transform->GetOrigin() + Vec3(0, 10, 0));
}
