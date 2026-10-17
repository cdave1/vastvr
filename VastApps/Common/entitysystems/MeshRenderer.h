#ifndef _VERTEX_MESH_RENDERER_H_
#define _VERTEX_MESH_RENDERER_H_

#include <Vast/Vast.h>

namespace hd {
    class EntityPool;
}

class MeshRenderer : public hd::EntitySystem {
public:
    
    MeshRenderer(hd::EntityPool *entityPool, hd::application::Client *client, const std::string entityName);

    void Update(hd::Entity *camera);

    void RenderOpaque(hd::Entity *camera, hd::input::EyePosition eyePosition);

    void RenderTransparent(hd::Entity *camera, hd::input::EyePosition eyePosition);

    void RenderLines(hd::Entity *camera, hd::input::EyePosition eyePosition);

    std::string m_entityName;

    hd::application::Client *m_client;

    hd::Vector<hd::Entity *> m_queryList;

};

#endif