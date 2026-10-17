#ifndef _VERTEX_INSTANCED_RENDERER_H_
#define _VERTEX_INSTANCED_RENDERER_H_

#include <Vast/Vast.h>

namespace hd {
    class EntityPool;
}

typedef struct MeshShaderPair {
    hd::graphics::MeshReference *mesh;
    hd::graphics::Shader *shader;
    hd::Texture *texture;
} MeshShaderPair;

class InstancedRenderer : public hd::EntitySystem {
public:
    
    InstancedRenderer(hd::EntityPool *entityPool, hd::application::Client *client, const std::string entityName);

    void Update(hd::Entity *camera);

    void RenderDepth(hd::Entity *camera, hd::input::EyePosition eyePosition);

    void RenderOpaque(hd::Entity *camera, hd::input::EyePosition eyePosition);

    void RenderTransparent(hd::Entity *camera, hd::input::EyePosition eyePosition);

    void RenderLines(hd::Entity *camera, hd::input::EyePosition eyePosition);

    std::string m_entityName;

    hd::application::Client *m_client;

    hd::Vector<hd::Entity *> m_queryList;

    hd::Vector<MeshShaderPair> m_meshIndex;

};

#endif