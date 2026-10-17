#ifndef _SKYBOX_RENDERER_H_
#define _SKYBOX_RENDERER_H_

#include <Vast/Vast.h>
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd {
    class EntityPool;
}

class SkyboxRenderer : public hd::EntitySystem {
public:
    
    SkyboxRenderer(hd::EntityPool *entityPool);
    
    void Update(hd::Entity *currentCamera);
    
    void Draw(hd::Entity *camera, hd::input::EyePosition eyePosition);
    
    float m_time;
    
    hd::sound::MusicPlayer *m_musicPlayer;
    
};

#endif