#ifndef _COLLISION_VOLUME_RENDERER_H_
#define _COLLISION_VOLUME_RENDERER_H_

#include <Vast/Vast.h>

namespace hd {
    class EntityPool;
}

class CollisionVolumeRenderer : public hd::EntitySystem {
public:
    
    CollisionVolumeRenderer(hd::EntityPool *entityPool, hd::application::Client *client, const std::string entityName);

    void Render(hd::Entity *camera, hd::input::EyePosition eyePosition);

    std::string m_entityName;

    hd::application::Client *m_client;
    
};

#endif