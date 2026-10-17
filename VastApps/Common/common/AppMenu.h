#ifndef _VAST_APPS_APPMENU_H_
#define _VAST_APPS_APPMENU_H_

#include "AppPrototype.h"

class AppMenu : public AppPrototype {
private:

    std::function<void(std::string appId)> m_onAppSelected;

public:
    AppMenu(hd::application::Client *client);

    void AddAppItem(const std::string &appId, const std::string &iconPath);

    void ShowMenu(hd::Entity *camera);

    void HideMenu(hd::Entity *camera);

    void OnMenuItemSelect(hd::Entity *element, const Collision3 &collision);
    
    void OnMenuItemDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);
    
    void OnMenuItemRelease(hd::Entity *element, const Collision3 &collision);
    
    void OnMenuItemReleaseInside(hd::Entity *element, const Collision3 &collision);

    template<class T>
    void OnAppSelected(T *src, void (T::*onAppSelected)(std::string appId));

    bool m_isShowing;

};


template<class T>
void AppMenu::OnAppSelected(T *src, void (T::*onAppSelected)(std::string appId)) {
    m_onAppSelected = std::bind(onAppSelected, src, std::placeholders::_1);
}

#endif
