#ifndef _UI_INPUT_HANDLER_H_
#define _UI_INPUT_HANDLER_H_

#include <Vast/Vast.h>

namespace hd {
    namespace application {
        class Client;
    }
    class EntityPool;
}

class hdGame;

class UiInputSystem : public hd::EntitySystem {
public:
    
    UiInputSystem(hd::EntityPool *entityPool, hd::application::Client *client);
    
    void Update();
    
private:
    
    void PointerSelectQuery(hd::Entity *event);
    
    void PointerDragQuery(hd::Entity *event);
    
    void PointerReleaseQuery(hd::Entity *event);
    
    hd::application::Client *m_client;
    
};

#endif