#ifndef _GEOMETRY_RENDERER_H_
#define _GEOMETRY_RENDERER_H_

#include <Vast/Vast.h>
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd {
    class EntityPool;
    class Entity;
}

class GeometryRenderer : public hd::EntitySystem {
public:
    
    GeometryRenderer(hd::EntityPool *entityPool);
    
    void UpdateGeometry(const hd::Entity *cameraEntity);
    
    hd::Entity *m_geometryShaderEntity;

    hd::Vector<hd::Entity *> m_queryList;

    hd::Vector<hd::graphics::MeshReference *> m_meshList;
    
};

#endif