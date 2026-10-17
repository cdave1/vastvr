#ifndef _APPS_SOUND_TEST_APP_H_
#define _APPS_SOUND_TEST_APP_H_

#include "AppPrototype.h"

class SoundTestApp : public AppPrototype {
public:
    SoundTestApp(hd::application::Client *client);

    ~SoundTestApp();

    void CreateSoundTestApp();

    void OnMenuItemSelect(hd::Entity *element, const Collision3 &collision);

    void OnMenuItemDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta);

    void OnMenuItemRelease(hd::Entity *element, const Collision3 &collision);

    void OnMenuItemReleaseInside(hd::Entity *element, const Collision3 &collision);
    
};

#endif
