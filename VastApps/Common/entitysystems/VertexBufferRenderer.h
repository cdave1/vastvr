#ifndef _VERTEX_BUFFER_RENDERER_H_
#define _VERTEX_BUFFER_RENDERER_H_

#include <Vast/Vast.h>

namespace hd {
    class EntityPool;
}

class VertexBufferRenderer : public hd::EntitySystem {
public:
    
    VertexBufferRenderer(hd::EntityPool *entityPool, hd::application::Client *client, const std::string entityName);

    void RenderOpaque(hd::Entity *camera, hd::input::EyePosition eyePosition);

    void RenderTransparent(hd::Entity *camera, hd::input::EyePosition eyePosition);

    std::string m_entityName;

    hd::application::Client *m_client;

};

#endif