#ifndef _LABEL_RENDERER_H_
#define _LABEL_RENDERER_H_

#include <Vast/Vast.h>

class LabelRenderer : public hd::EntitySystem {
public:
    
    LabelRenderer(hd::EntityPool *entityPool);
    
    void Update(hd::Entity *camera, hd::input::EyePosition eyePosition);
    
    float m_time;
    
    hd::sound::MusicPlayer *m_musicPlayer;
    
};

#endif