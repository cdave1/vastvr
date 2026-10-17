#ifndef _VAST_DASHBOARD_MENU_H_
#define _VAST_DASHBOARD_MENU_H_

#include "common/AppPrototype.h"
#include "entitysystems/Cameras.h"
#include "InterfaceUtils.h"

#define DASHBOARD_PANEL_NAME "DashboardPanel"
#define DASHBOARD_MENU_ITEM_NAME "DashboardItem"
#define DASHBOARD_BUTTON_NAME "DashboardButton"
#define DASHBOARD_LABEL_NAME "label"
#define DASHBOARD_COLOR_ITEM_NAME "ColorItem"

class Builder;

class DashboardMenu {
private:

    hd::Entity * m_selectedShapeMenuItem;

    hd::ui::SphereLayout *m_sphereLayout;

    hd::Entity *m_target;

    hd::Entity *m_currentColorPanel;

    Builder *m_builder;

    Color4f m_selectedHue;

    Color4f m_selectedColor;

    //std::function<void(std::string appId)> m_onAppSelected;

public:
    DashboardMenu(Builder *builder, hd::EntityPool *entityPool);

    void CreateMainMenu(hd::EntityPool *entityPool);

    void CreateGridOptionsMenu(hd::EntityPool *entityPool);

    void CreateMeshSelectionMenu(hd::EntityPool *entityPool);

    void CreateTextureMenu(hd::EntityPool *pool);

    void ToggleMenu();

    bool IsMenuVisible() const;

    void RepositionMenu(hd::EntityPool *entityPool, hd::Entity *camera);

    void HideMenu(hd::EntityPool *entityPool, hd::Entity *camera);

#pragma mark - Pointer functions for interacting with menu geometry.

    void OnPointerMove(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);

    void OnPointerSelect(hd::Entity *element, const Collision3 &collision);
    
    void OnPointerDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);
    
    void OnPointerRelease(hd::Entity *element, const Collision3 &collision);
    
    void OnPointerReleaseInside(hd::Entity *element, const Collision3 &collision);


#pragma mark - Button pointer functions.

    void OnHueSelect(hd::Entity *element, const Collision3 &collision);

    void OnHueDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);

    void OnHueRelease(hd::Entity *element, const Collision3 &collision);


    void OnColorSelect(hd::Entity *element, const Collision3 &collision);

    void OnColorDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);

    void OnColorRelease(hd::Entity *element, const Collision3 &collision);


    void OnLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void OnLookAtHover(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);

    void OnLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision);


private:

    void UpdatePickerColor(hd::Entity *element, const Collision3 &collision, bool hue);

    static void ShowPanel(hd::Entity *item);

    static void HidePanel(hd::Entity *item);


};

#endif
