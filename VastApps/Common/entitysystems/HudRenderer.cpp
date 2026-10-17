#include "HudRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

HudRenderer::HudRenderer(EntityPool *entityPool) : EntitySystem(entityPool) {}


void HudRenderer::Update() {
	EntityQuery *hudItems = m_entityPool->Query<hd::graphics::Polygon, PolygonRenderer, ScreenData, Transform>();
    
    for (int i = 0; i < hudItems->Size(); ++i) {
        Entity *entity = hudItems->At(i);
        
        auto polygon = entity->GetComponent<graphics::Polygon>();
        auto renderer = entity->GetComponent<graphics::PolygonRenderer>();
        auto screen = entity->GetComponent<graphics::ScreenData>();
        auto transform = entity->GetComponent<graphics::Transform>();
        
        screen->RenderOrthographic();
        
        if (entity->ContainsComponent<graphics::Material>()) {
            auto mat = entity->GetComponent<graphics::Material>();
            //renderer->RenderMaterial(polygon, mat);
        }
        
        renderer->RenderLineStrip(polygon);
    }
}