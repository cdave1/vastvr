#include "AppMenu.h"

static AABB worldSize(0, 0, 1024, 512);

AppMenu::AppMenu(hd::application::Client *client) : AppPrototype(client) {
}


const float RADIUS = 100.0f;

void AppMenu::AddAppItem(const std::string &appId, const std::string &iconPath) {
    Entity *menuItem = AddMenuItem(Vec3(), 0.0f, iconPath, appId, RADIUS);
    auto selectResponder = menuItem->Edit<hd::input::pointer::SelectResponder>();
    auto dragResponder = menuItem->Edit<hd::input::pointer::DragResponder>();
    auto releaseResponder = menuItem->Edit<hd::input::pointer::ReleaseResponder>();

    selectResponder->OnSelect(this, &AppMenu::OnMenuItemSelect);
    dragResponder->OnDrag(this, &AppMenu::OnMenuItemDrag);
    releaseResponder->OnRelease(this, &AppMenu::OnMenuItemRelease);
    releaseResponder->OnReleaseInside(this, &AppMenu::OnMenuItemReleaseInside);

    auto opacity = menuItem->Edit<hd::graphics::OpacityData>();
    opacity->SetOpacity(0.0f);
}


void AppMenu::OnMenuItemSelect(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Select: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetScale(1.2f);
}


void AppMenu::OnMenuItemDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    hd::log::i("Drag: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetOrigin(transform->GetOrigin() + delta);
}


void AppMenu::OnMenuItemRelease(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Release: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetScale(1.0f);
 
    //m_musicPlayer.PlayMusic("/Users/davidpetrie/Projects/dmg/VastBrowser/resources/sfx/blop_001.wav");
    
    auto identifier = element->GetComponent<hd::ui::Identifier>();
    if (m_onAppSelected) {
        m_onAppSelected(identifier->GetId());
    }
    std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
    hd::sound::SoundController::PlayEffect(path.c_str());
}


void AppMenu::OnMenuItemReleaseInside(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Release Inside: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetScale(0.9f);
}


void AppMenu::ShowMenu(hd::Entity *camera) {
    EntityQuery *blocks = GetEntityPool()->QueryByName("Block");
    auto headset = camera->GetComponent<hd::input::Headset>();

    Vec3 location(0.0f, 0.0f, 400.0f);
    Vec3 startLocation(0.0f, 4000.0f, 20000.0f);

    Quaternion rotation = headset->GetWorldOrientation();
    Quaternion rotationAdjustment(Vec3(0, 1, 0), - (hd_half_pi * 0.5f));
    rotation = rotation * rotationAdjustment;

    location = rotation * location;
    startLocation = rotation * startLocation;

    Quaternion step(Vec3(0, 1, 0), (hd_half_pi / 4.5));

    for (int i = 0; i < blocks->Size(); ++i) {
        auto menuItem = blocks->At(i);

        Vec3 adjustment = Vec3(0.0f, ((i % 2) * RADIUS), 0.0f);

        auto timer = menuItem->Edit<hd::animation::AnimationTimer>();
        auto positionBlock = menuItem->Edit<hd::animation::PositionBlock>();
        auto opacityBlock = menuItem->Edit<hd::animation::OpacityBlock>();

        if (timer) {
            if (timer->IsActive()) {
                timer->Stop();
            }
            
            Quaternion rot = rotation;
            rot.Invert();
            auto transform = menuItem->Edit<hd::graphics::Transform>();
            transform->SetRotation(rot * Quaternion(Vec3(0, 1, 0), hd_pi));

            positionBlock->SetStartPosition(headset->GetWorldPosition() + startLocation);
            positionBlock->SetEndPosition(headset->GetWorldPosition() + location + adjustment);
            opacityBlock->SetStartOpacity(0.0f);
            opacityBlock->SetEndOpacity(1.0f);

            timer->SetDuration(0.3f);
            timer->Start();
        }

        location = step * location;
        startLocation = step * startLocation;
        rotation = rotation * step;
    }

    std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_004.wav");
    hd::sound::SoundController::PlayEffect(path.c_str());
}


void AppMenu::HideMenu(hd::Entity *camera) {
    EntityQuery *blocks = GetEntityPool()->QueryByName("Block");

    for (int i = 0; i < blocks->Size(); ++i) {
        auto menuItem = blocks->At(i);

        auto timer = menuItem->Edit<hd::animation::AnimationTimer>();
        auto positionBlock = menuItem->Edit<hd::animation::PositionBlock>();
        auto opacityBlock = menuItem->Edit<hd::animation::OpacityBlock>();

        if (timer) {
            if (timer->IsActive()) {
                timer->Stop();
            }
            positionBlock->Reverse();
            opacityBlock->Reverse();
            timer->Start();
        }
    }

    std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_008.wav");
    hd::sound::SoundController::PlayEffect(path.c_str());
}
