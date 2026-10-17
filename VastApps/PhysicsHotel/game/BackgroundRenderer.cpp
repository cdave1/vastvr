#include "BackgroundRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

BackgroundRenderer::BackgroundRenderer(EntityPool *entityPool) : EntitySystem(entityPool), m_time(0.0f) {}


void BackgroundRenderer::Update() {
    m_time += 0.016f;
    
    EntityQuery *blocks = m_entityPool->Query<graphics::VertexBuffer, graphics::Shader>();
    return;
	for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);
        
        graphics::WorldData *worldData = entity->GetComponent<hd::graphics::WorldData>();
        
        if (!entity->ContainsComponent<graphics::PolygonRenderer>()) {
            auto buffer = entity->GetComponent<graphics::VertexBuffer>();
            auto shader = entity->GetComponent<graphics::Shader>();
            
            worldData->StartRender();
            shader->Begin();
            
            if (entity->ContainsComponent<graphics::Material>()) {
                entity->GetComponent<graphics::Material>()->BindTexture();
            }
            
            shader->SetUniform1f("iGlobalTime", 0.1f * m_time);
            shader->SetUniform3f("iResolution", Vec3(640, 320, 0));
            buffer->Render();
            shader->End();
            
            worldData->EndRender();
        }
    }
}