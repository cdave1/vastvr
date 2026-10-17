#ifndef _BLOCK_INPUT_HANDLER_H_
#define _BLOCK_INPUT_HANDLER_H_

#include "hdk.h"
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd {
    namespace application {
        class Client;
    }
    class EntityPool;
}

class hdGame;

class BlockInputSystem : public hd::EntitySystem {
public:
    
    BlockInputSystem(hd::EntityPool *entityPool, hd::application::Client *client);
    
    void Update(hd::Entity *currentCamera);
    
    hdGame *m_game;
    
private:
    
    void PointerSelectQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void PointerDragQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    void PointerReleaseQuery(hd::Entity *event, hd::Entity *currentCamera);
    
    hd::application::Client *m_client;
    
};

#endif