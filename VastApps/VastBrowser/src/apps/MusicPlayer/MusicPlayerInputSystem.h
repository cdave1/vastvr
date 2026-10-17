#ifndef _APPS_MUSICPLAYER_SYSTEM_H_
#define _APPS_MUSICPLAYER_SYSTEM_H_

#include <Vast/Vast.h>

namespace hd {
    namespace application {
        class Client;
    }
    class EntityPool;
}


class MusicPlayerInputSystem : public hd::EntitySystem {
public:

    MusicPlayerInputSystem(hd::EntityPool *entityPool, hd::application::Client *client);

    void KeyboardQuery(hd::Entity *event);

    void Update();

    void Render();


private:

    hd::application::Client *m_client;

};

#endif
