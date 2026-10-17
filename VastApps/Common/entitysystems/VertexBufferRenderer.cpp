#include "VertexBufferRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

VertexBufferRenderer::VertexBufferRenderer(EntityPool *entityPool,
                                           hd::application::Client *client,
                                           const std::string entityName) :
    EntitySystem(entityPool),
    m_client(client),
    m_entityName(entityName)
{}


void VertexBufferRenderer::RenderOpaque(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    auto headset = camera->GetComponent<hd::input::Headset>();

    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();

//    for (int i = 0; i < lights->Size(); ++i) {
//        Entity *lightEntity = lights->At(i);

    EntityQuery *blocks = m_entityPool->Query<graphics::Shader, graphics::VertexBuffer, graphics::Material>(m_entityName);
    hd::Vector<Entity *> opaqueItems;

    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
	for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);

        auto opacityData = entity->GetComponent<hd::graphics::OpacityData>();

        if (opacityData && opacityData->GetOpacity() < 1.0) {
            continue;
        }

        auto transform = entity->GetComponent<hd::graphics::Transform>();
        auto buffer = entity->GetComponent<graphics::VertexBuffer>();
        auto shader = entity->Edit<graphics::Shader>();
        auto mat = entity->GetComponent<graphics::Material>();

        shader->Begin();
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

        for (int j = 0; j < lights->Size(); ++j) {
            Entity *lightEntity = lights->At(0);
            auto light = lightEntity->Get<graphics::Light>();
            LightRenderer::SetLightUniforms(light, shader);
        }

        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, headset->GetLookAtMatrix(eyePosition));
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, headset->GetProjection(eyePosition));
        shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
        buffer->Render();
        shader->End();
    }
}


void VertexBufferRenderer::RenderTransparent(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    auto headset = camera->GetComponent<hd::input::Headset>();

    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();

    EntityQuery *blocks = m_entityPool->Query<graphics::VertexBuffer>(m_entityName);
    hd::Vector<Entity *> opaqueItems;
    hd::Vector<Entity *> transparentItems;

    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);

    //glEnable(GL_BLEND);
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);

        auto opacityData = entity->GetComponent<hd::graphics::OpacityData>();

        if (!opacityData || opacityData->GetOpacity() == 1.0 || opacityData->GetOpacity() == 0.0) {
            continue;
        }

        auto transform = entity->GetComponent<hd::graphics::Transform>();
        auto buffer = entity->GetComponent<graphics::VertexBuffer>();
        auto mat = entity->GetComponent<graphics::Material>();

        auto shader = entity->Edit<graphics::Shader>();
        shader->Begin();
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

        for (int j = 0; j < lights->Size(); ++j) {
            Entity *lightEntity = lights->At(0);
            auto light = lightEntity->Get<graphics::Light>();
            LightRenderer::SetLightUniforms(light, shader);
        }

        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, opacityData->GetOpacity());
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, headset->GetLookAtMatrix(eyePosition));
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, headset->GetProjection(eyePosition));
        shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
        buffer->Render();
        shader->End();
    }
   // glDisable(GL_BLEND);
}
