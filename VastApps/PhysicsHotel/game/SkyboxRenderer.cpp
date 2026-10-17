#include "SkyboxRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

SkyboxRenderer::SkyboxRenderer(EntityPool *entityPool) : EntitySystem(entityPool), m_time(0.0f) {}


void SkyboxRenderer::Update() {
    m_time += 0.016f;
    
    const std::string SKYBOX_NAME = "sphere";
    const std::string MAIN_CAMERA = "mainCamera";
    
    EntityQuery *skyboxes = m_entityPool->QueryByName(SKYBOX_NAME);
    EntityQuery *cameras = m_entityPool->QueryByName(MAIN_CAMERA);
    
    assert(skyboxes->Size() > 0);
    assert(cameras->Size() > 0);
    
    auto mainCamera = cameras->At(0);
    
	for (int i = 0; i < skyboxes->Size(); ++i) {
        Entity *entity = skyboxes->At(i);
        
        auto cameraData = mainCamera->GetComponent<CameraData>();
        
        auto worldData = entity->GetComponent<hd::graphics::WorldData>();
        auto buffer = entity->GetComponent<graphics::VertexBuffer>();
        auto shader = entity->GetComponent<graphics::Shader>();
        
        worldData->SetOrigin(cameraData->GetLocation());
        worldData->StartRender();
        
        shader->Begin();
        if (entity->ContainsComponent<graphics::Material>()) {
            entity->GetComponent<graphics::Material>()->BindTexture();
        }
        shader->SetUniform1f("iGlobalTime", 0.1f * m_time);
        shader->SetUniform3f("iResolution", Vec3(10000, 6000, 0));
        buffer->Render();
        shader->End();
        
        worldData->EndRender();
    }
}