#ifndef _BACKGROUND_RENDERER_H_
#define _BACKGROUND_RENDERER_H_

#include <Vast/Vast.h>
#include <Vast/VastCore.h>
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd {
    class EntityPool;
}

class BackgroundRenderer : public hd::EntitySystem {
public:
    
    BackgroundRenderer(hd::EntityPool *entityPool);
    
    void Update();
    
    float m_time;
    
    hd::sound::MusicPlayer *m_musicPlayer;
    
};

#endif