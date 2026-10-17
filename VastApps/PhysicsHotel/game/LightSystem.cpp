#include "LightSystem.h"

using namespace hd;

LightSystem::LightSystem(EntityPool *entityPool) : EntitySystem(entityPool) {}

static float f = 0.0f;
void LightSystem::Update() {

    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();
    
	for (int i = 0; i < lights->Size(); ++i) {
        Entity *lightEntity = lights->At(i);
        
        //auto light = lightEntity->GetComponent<hd::graphics::Light>();
        //auto lightModel = lightEntity->GetComponent<hd::graphics::LightModel>();
        //auto lightRenderer = lightEntity->GetComponent<hd::graphics::LightRenderer>();
        auto worldData = lightEntity->GetComponent<hd::graphics::WorldData>();
        
        Matrix rot;
        Matrix::RotationY(rot, 0.01f);
        Vec3 origin = worldData->GetOrigin();
        Matrix::Vec3Multiply(origin, origin, rot);
        worldData->SetOrigin(origin);
        
        /*
        worldData->StartRender();
        
        lightRenderer->RenderLightModel(NULL);
        lightRenderer->RenderLight(light, worldData);
        
        worldData->EndRender();*/
    }
    
    f += 0.01;
}


void LightSystem::Begin() {
    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();
    
	for (int i = 0; i < lights->Size(); ++i) {
        Entity *lightEntity = lights->At(i);
        
        auto light = lightEntity->GetComponent<hd::graphics::Light>();
        //auto lightModel = lightEntity->GetComponent<hd::graphics::LightModel>();
        auto lightRenderer = lightEntity->GetComponent<hd::graphics::LightRenderer>();
        auto worldData = lightEntity->GetComponent<hd::graphics::WorldData>();
        
        worldData->StartRender();
        
        lightRenderer->Begin();
        lightRenderer->RenderLightModel(NULL);
        lightRenderer->RenderLight(light, worldData);
        
        worldData->EndRender();
    }
}


void LightSystem::End() {
    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();
    
	for (int i = 0; i < lights->Size(); ++i) {
        Entity *lightEntity = lights->At(i);
        
        auto lightRenderer = lightEntity->GetComponent<hd::graphics::LightRenderer>();
        lightRenderer->End();
    }
}
