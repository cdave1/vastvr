#include <vector>
#include "SoundTestApp.h"

static AABB worldSize(0, 0, 2048, 512);

SoundTestApp::SoundTestApp(hd::application::Client *client) : AppPrototype(client) {
    m_skyboxRenderer.m_musicPlayer = &m_musicPlayer;
    m_blockMover.m_musicPlayer = &m_musicPlayer;

    CreateSoundTestApp();

    m_blockMover.m_musicPlayer = &m_musicPlayer;
}


void SoundTestApp::CreateSoundTestApp() {

    // No music is distributed. Add paths to your own licensed audio here.
    const std::vector<std::string> identifiers;
    const int MENU_ITEM_COUNT = static_cast<int>(identifiers.size());

    for (int i = 0; i < MENU_ITEM_COUNT; ++i) {
        Vec3 location = Vec3(0.0f + (600.0f * i), 500.0f, -1024.0f);
        Entity *menuItem = AddMenuItem(location, 0.0f, identifiers[i], "resources/images/music_player.png");
        auto selectResponder = menuItem->Edit<hd::input::pointer::SelectResponder>();
        auto releaseResponder = menuItem->Edit<hd::input::pointer::ReleaseResponder>();

        selectResponder->OnSelect(this, &SoundTestApp::OnMenuItemSelect);
        releaseResponder->OnRelease(this, &SoundTestApp::OnMenuItemRelease);
        releaseResponder->OnReleaseInside(this, &SoundTestApp::OnMenuItemReleaseInside);
    }

    ConstructSkybox();
    CreateLights();
}


void SoundTestApp::OnMenuItemSelect(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Select: %d\n", element->GetID());
    auto worldData = element->Edit<hd::graphics::Transform>();
    worldData->SetScale(1.2f);
}


void SoundTestApp::OnMenuItemRelease(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Release: %d\n", element->GetID());
    auto worldData = element->Edit<hd::graphics::Transform>();
    worldData->SetScale(1.0f);

    auto identifier = element->Edit<hd::ui::Identifier>();
    std::string path = hd::FileSystem::FullPathTo(identifier->GetId());
    m_musicPlayer.PlayMusic(path.c_str());
}


void SoundTestApp::OnMenuItemReleaseInside(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Release Inside: %d\n", element->GetID());
    auto worldData = element->Edit<hd::graphics::Transform>();
    worldData->SetScale(0.9f);

    auto identifier = element->Edit<hd::ui::Identifier>();
    std::string path = hd::FileSystem::FullPathTo(identifier->GetId());
    m_musicPlayer.PlayMusic(path.c_str());
}