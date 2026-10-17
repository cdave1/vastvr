#include "BackgroundRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

BackgroundRenderer::BackgroundRenderer(EntityPool *entityPool) : EntitySystem(entityPool), m_time(0.0f), m_musicPlayer(NULL) {}


void BackgroundRenderer::Update() {
    m_time += 0.016f;
    
    const std::string BACKGROUND_NAME = "background";
    EntityQuery *backgrounds = m_entityPool->QueryByName(BACKGROUND_NAME);
    
	for (int i = 0; i < backgrounds->Size(); ++i) {
        Entity *entity = backgrounds->At(i);
        
        auto buffer = entity->Edit<graphics::VertexBuffer>();
        auto shader = entity->Edit<graphics::Shader>();
        
        shader->Begin();
        
        if (entity->ContainsComponent<graphics::Material>()) {
            auto material = entity->GetComponent<graphics::Material>();
            shader->BindTexture("iTexture0", material->GetTexture());
        }
        
        shader->SetUniform1i("uAmplitudeSize", hd::min((unsigned int)512, m_musicPlayer->GetAmplitudesCount()));
        shader->SetUniform1fv("uAmplitudeSet", hd::min((unsigned int)512, m_musicPlayer->GetAmplitudesCount()), m_musicPlayer->GetAmplitudesPointer());
        shader->SetUniform1f("uAmplitude", m_musicPlayer->GetAverageAmplitude());
        
        shader->SetUniform1f("iGlobalTime", 0.1f * m_time);
        shader->SetUniform3f("iResolution", Vec3(640, 320, 0));
        buffer->Render();
        shader->End();
    }
}