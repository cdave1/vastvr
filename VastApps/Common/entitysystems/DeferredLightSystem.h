#ifndef _DEFERRED_LIGHTING_SYSTEM_H_
#define _DEFERRED_LIGHTING_SYSTEM_H_

#include <Vast/Vast.h>

namespace hd {
    class EntityPool;
}

class DeferredLightSystem : public hd::EntitySystem {
public:
    
    DeferredLightSystem(hd::EntityPool *entityPool, hd::application::Client *client);

    void HandleInput();
    
    void Update(hd::Entity *camera);
    
    const hd::Texture * GetShadowTexture();

    Vec4 GetLightPositionWorldSpace();
    
    Matrix GetLightProjectionMatrix();
    
    Matrix GetLightModelViewMatrix();

    float m_rotationDelta;

    hd::application::Client *m_client;
    
};

#endif