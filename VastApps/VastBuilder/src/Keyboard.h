#ifndef _VAST_KEYBOARD_H_
#define _VAST_KEYBOARD_H_

#include "common/AppPrototype.h"

#define KEYBOARD_PANEL_NAME "KeyboardPanel"
#define KEYBOARD_MENU_ITEM_NAME "KeyboardItem"
#define KEYBOARD_BUTTON_NAME "KeyboardButton"
#define KEYBOARD_LABEL_NAME "label"

class Builder;

class Keyboard {
public:
    Keyboard(Builder *builder, hd::EntityPool *entityPool);

    hd::Entity * AddKey(hd::EntityPool *entityPool, const std::string text, const float row, const float column);

    void Show(hd::EntityPool *entityPool, hd::Entity *camera);

    void Hide(hd::EntityPool *entityPool, hd::Entity *camera);

    void OnSelect(hd::Entity *element, const Collision3 &collision);
    
    void OnDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);
    
    void OnRelease(hd::Entity *element, const Collision3 &collision);
    
    void OnReleaseInside(hd::Entity *element, const Collision3 &collision);

    void OnLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void OnLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

};

#endif
