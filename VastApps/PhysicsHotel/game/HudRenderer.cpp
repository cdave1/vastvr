#include "HudRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

HudRenderer::HudRenderer(EntityPool *entityPool) : EntitySystem(entityPool) {}


void HudRenderer::Update() {
    EntityQuery *hudItems = m_entityPool->Query<Polygon, PolygonRenderer, ScreenData, WorldData>();
    
    for (int i = 0; i < hudItems->Size(); ++i) {
        Entity *entity = hudItems->At(i);
        
        graphics::Polygon *polygon = entity->GetComponent<graphics::Polygon>();
        graphics::PolygonRenderer *renderer = entity->GetComponent<graphics::PolygonRenderer>();
        graphics::ScreenData *screen = entity->GetComponent<graphics::ScreenData>();
        graphics::WorldData *worldData = entity->GetComponent<graphics::WorldData>();
        
        screen->RenderOrthographic();
        worldData->StartRender();
        
        if (entity->ContainsComponent<graphics::Material>()) {
            graphics::Material *mat = entity->GetComponent<graphics::Material>();
            //renderer->RenderMaterial(polygon, mat);
        }
        
        renderer->RenderLineStrip(polygon);
        worldData->EndRender();
    }
}