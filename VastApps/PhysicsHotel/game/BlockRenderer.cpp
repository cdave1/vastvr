#include "BlockRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

BlockRenderer::BlockRenderer(EntityPool *entityPool) : EntitySystem(entityPool), m_time(0.0f) {}


void BlockRenderer::Update() {
    m_time += 0.016f;
    
    const std::string BLOCK_NAME = "block";
    EntityQuery *blocks = m_entityPool->QueryByName(BLOCK_NAME);
    
	for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);
        
        auto worldData = entity->GetComponent<hd::graphics::WorldData>();
        auto polygon = entity->GetComponent<graphics::Polygon>();
        auto renderer = entity->GetComponent<graphics::PolygonRenderer>();
        auto buffer = entity->GetComponent<graphics::VertexBuffer>();
        auto shader = entity->GetComponent<graphics::Shader>();
        auto mat = entity->GetComponent<graphics::Material>();
        
        if (entity->ContainsComponent<pointer::ResponderState>()) {
            auto *responderState = entity->GetComponent<pointer::ResponderState>();
            if (responderState->GetCurrentState() == pointer::FREE) {
                worldData->SetScale(1.0f);
            } else if (responderState->GetCurrentState() == pointer::SELECTED) {
                worldData->SetScale(1.2f);
            } else if (responderState->GetCurrentState() == pointer::DRAGGED) {
                worldData->SetScale(1.3f);
            } else {
                worldData->SetScale(1.0f);
            }
        }
        
        worldData->StartRender();
        shader->Begin();
        
        if (mat) {
            mat->BindTexture();
            mat->Render();
        } else {
            hd::gl::BindTexture(NULL);
        }
        
        shader->SetUniform3f("iResolution", Vec3(1000, 600, 0));
        shader->SetUniform1f("iGlobalTime", m_time);
        buffer->Render();
        shader->End();
        
        worldData->EndRender();
    }
}