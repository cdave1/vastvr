#include "HomeSpace.h"

const std::string CONTEXT_MENU_ENTITY_NAME = "ContextMenuItems";

HomeSpace::HomeSpace(hd::application::Client *client) {
}


void HomeSpace::OnMenuItemSelect(hd::Entity *element, const Vec3 &position) {
    hd::log::i("Select: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetOrigin(transform->GetOrigin() + Vec3(0.0f, -10.0f, 0.0f));
}


void HomeSpace::OnMenuItemDrag(hd::Entity *element, const Vec3 &position, const Vec3 &delta) {
    hd::log::i("Drag: %d\n", element->GetID());
//    auto transform = element->GetComponent<hd::graphics::Transform>();
//    transform->SetOrigin(transform->GetOrigin() + delta);
}


void HomeSpace::OnMenuItemRelease(hd::Entity *element, const Vec3 &position) {
    hd::log::i("Release: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetScale(1.0f);
    transform->SetOrigin(transform->GetOrigin() + Vec3(0.0f, 10.0f, 0.0f));
}


void HomeSpace::OnMenuItemReleaseInside(hd::Entity *element, const Vec3 &position) {
    hd::log::i("Release Inside: %d\n", element->GetID());
    //auto transform = element->GetComponent<hd::graphics::Transform>();
    //transform->SetOrigin(transform->GetOrigin() + Vec3(0.0f, 20.0f, 0.0f));
}
