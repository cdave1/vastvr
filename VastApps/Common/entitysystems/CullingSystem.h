#ifndef _CULLING_SYSTEM_H_
#define _CULLING_SYSTEM_H_

#include <Vast/Vast.h>

namespace hd {
    class EntityPool;
}

class CullingSystem : public hd::EntitySystem {
public:
    
    CullingSystem(hd::EntityPool *entityPool, hd::application::Client *client);

    void Update(hd::Entity *camera);

    hd::application::Client *m_client;

};

#endif