#ifndef _APPS_HOME_SPACE_H_
#define _APPS_HOME_SPACE_H_

#include <Vast/Vast.h>

///
/// Home menu that people see when they first load the app.
///
/// Contains static buttons (as meshes) for the following things:
/// -- tutorial (v4)
/// -- create a new space (v1)
/// -- load an existing space (v1)
/// -- Friend and their worlds (v2)
/// -- news and updates (v3)
///
class HomeSpace {
public:
    HomeSpace(hd::application::Client *client);
    
    ~HomeSpace();

    void OnMenuItemSelect(hd::Entity *element, const Vec3 &position);

    void OnMenuItemDrag(hd::Entity *element, const Vec3 &position, const Vec3 &delta);

    void OnMenuItemRelease(hd::Entity *element, const Vec3 &position);

    void OnMenuItemReleaseInside(hd::Entity *element, const Vec3 &position);
    
};

#endif
