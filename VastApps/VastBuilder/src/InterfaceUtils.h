#ifndef _VAST_INTERFACE_UTILS_H_
#define _VAST_INTERFACE_UTILS_H_

#include "common/AppPrototype.h"
#include "entitysystems/Cameras.h"

#define DASHBOARD_PANEL_NAME "DashboardPanel"
#define DASHBOARD_MENU_ITEM_NAME "DashboardItem"
#define DASHBOARD_BUTTON_NAME "DashboardButton"
#define DASHBOARD_LABEL_NAME "label"
#define DASHBOARD_COLOR_ITEM_NAME "ColorItem"

class Builder;

class InterfaceUtils {
public:

    static hd::Entity * AddMenuItem(hd::EntityPool *entityPool, const std::string text, const Vec2 &size);

    static hd::Entity * AddTextureMenuItem(hd::EntityPool *pool, hd::Texture *texture, const std::string labelText, const Vec2 &size);

    static hd::Entity * AddBooleanOptionButton(hd::EntityPool *pool, const std::string labelText, const Vec2 &size);

    static hd::Entity * AddBackground(hd::EntityPool *entityPool, const float radius, const Vec2 &gridSize);

    static hd::Entity * AddColorPicker(hd::EntityPool *pool, const Vec2 &gridSize, hd::Entity **colorBrightnessPanel);

    static hd::Entity * AddHuePicker(hd::EntityPool *pool, const Vec2 &gridSize);

    static hd::Entity * AddColorPanel(hd::EntityPool *pool, hd::Entity **colorPickerPanel);

};

#endif
