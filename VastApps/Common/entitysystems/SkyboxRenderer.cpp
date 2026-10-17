#include "SkyboxRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

const std::string SKYBOX_NAME = "sphere";
const std::string MAIN_CAMERA = "mainCamera";

SkyboxRenderer::SkyboxRenderer(EntityPool *entityPool) : EntitySystem(entityPool), m_time(0.0f), m_musicPlayer(NULL) {}


void SkyboxRenderer::Update(hd::Entity *currentCamera) {
    EntityQuery *skyboxes = m_entityPool->QueryByName(SKYBOX_NAME);
    
	for (int i = 0; i < skyboxes->Size(); ++i) {
        Entity *entity = skyboxes->At(i);
        
        auto headset = currentCamera->GetComponent<Headset>();
        auto transform = entity->Edit<hd::graphics::Transform>();
        
        transform->SetOrigin(headset->GetWorldPosition());
    }
}


void SkyboxRenderer::Draw(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    m_time += 0.016f;
    auto headset = camera->GetComponent<hd::input::Headset>();
    EntityQuery *skyboxes = m_entityPool->QueryByName(SKYBOX_NAME);
    
	for (int i = 0; i < skyboxes->Size(); ++i) {
        Entity *entity = skyboxes->At(i);
        
        auto transform = entity->GetComponent<hd::graphics::Transform>();
        auto meshRef = entity->GetComponent<graphics::MeshReference>();
        auto mat = entity->GetComponent<graphics::Material>();
                auto shader = entity->Edit<graphics::Shader>();

        shader->Begin();

        if (m_musicPlayer) {
            shader->SetUniform1i("uAmplitudeSize", m_musicPlayer->GetAmplitudesCount());
            shader->SetUniform1fv("uAmplitudeSet", m_musicPlayer->GetAmplitudesCount(), m_musicPlayer->GetAmplitudesPointer());
            shader->SetUniform1f("uAmplitude", m_musicPlayer->GetAverageAmplitude());
        }

        if (mat) {
            if (mat->GetTexture()) {
                shader->BindTexture("iTexture0", mat->GetTexture());
            }
            if (mat->GetNormalTexture()) {
                shader->BindTexture("iNormalTexture0", mat->GetNormalTexture());
            }
        } else {
            hd::gl::BindTexture(NULL);
        }

        LightRenderer::SetMaterialUniforms(mat, shader);

        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, headset->GetLookAtMatrix(eyePosition));
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, headset->GetProjection(eyePosition));
        shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
        shader->SetUniform1f("iGlobalTime", 2.0f * m_time);
        shader->SetUniform2f("iResolution", 1.0, 0.7);
        shader->SetUniform4f("ColorTint", Vec4(0.0f, 0.3f, 0.6f, 1.0f));
        meshRef->Render();
        shader->End();
    }
}