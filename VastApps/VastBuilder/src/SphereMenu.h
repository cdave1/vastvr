#ifndef _VAST_SPHERE_MENU_H_
#define _VAST_SPHERE_MENU_H_

#include "common/AppPrototype.h"

#define SPHERE_PANEL_NAME "SpherePanel"
#define SPHERE_MENU_ITEM_NAME "SphereItem"
#define SPHERE_BUTTON_NAME "SphereButton"
#define SPHERE_LABEL_NAME "label"
#define SPHERE_COLOR_ITEM_NAME "ColorItem"

class Builder;

class SphereMenu {
public:
    SphereMenu(Builder *builder, hd::EntityPool *entityPool);

    hd::Entity * AddMenuItem(hd::EntityPool *entityPool, const std::string text);

    void ShowMenu(hd::EntityPool *entityPool, hd::Entity *camera);

    void RepositionMenu(hd::EntityPool *entityPool, hd::Entity *camera);

    void HideMenu(hd::EntityPool *entityPool, hd::Entity *camera);

    void OnSelect(hd::Entity *element, const Collision3 &collision);
    
    void OnDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);
    
    void OnRelease(hd::Entity *element, const Collision3 &collision);
    
    void OnReleaseInside(hd::Entity *element, const Collision3 &collision);

    void OnLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void OnLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);


private:

    void LayoutMenu(hd::EntityPool *entityPool, int len, int height, const Vec2 &arcLength);

};

#endif
