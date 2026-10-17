#include "LabelRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

LabelRenderer::LabelRenderer(EntityPool *entityPool) : EntitySystem(entityPool), m_time(0.0f), m_musicPlayer(NULL) {}


void LabelRenderer::Update(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    m_time += 0.016f;

    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();

    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
    Matrix viewMatrix = headset->GetLookAtMatrix(eyePosition);
    Matrix projectionMatrix = headset->GetProjection(eyePosition);

    glDisable(GL_CULL_FACE);

    const std::string LABEL_NAME = "label";
    EntityQuery *labels = m_entityPool->QueryByName(LABEL_NAME);
	for (int i = 0; i < labels->Size(); ++i) {
        Entity *entity = labels->At(i);

        auto transform = entity->Get<hd::graphics::Transform>();
        auto opacity = entity->Get<hd::graphics::OpacityData>();
        auto mat = entity->Get<graphics::Material>();
        auto font = entity->Get<ui::Font>();
        auto fontMesh = entity->Get<ui::FontMesh>();
        auto vertexBuffer = entity->Get<graphics::VertexBuffer>();
        auto layout = entity->Get<hd::ui::FontLayout>();
        auto text = entity->Get<ui::Text>();

        auto shader = entity->Edit<graphics::Shader>();

        if (opacity && opacity->IsVisible() == false) {
            continue;
        }

        shader->Begin();
        if (mat) {
            if (mat->GetTexture()) {
                shader->BindTexture("iTexture0", mat->GetTexture());
            }
            if (mat->GetNormalTexture()) {
                shader->BindTexture("iNormalTexture0", mat->GetNormalTexture());
            }
            mat->Render();
        } else {
            hd::gl::BindTexture(NULL);
        }

        LightRenderer::SetMaterialUniforms(mat, shader);
        for (int j = 0; j < lights->Size(); ++j) {
            Entity *lightEntity = lights->At(0);
            auto light = lightEntity->Get<graphics::Light>();
            LightRenderer::SetLightUniforms(light, shader);
        }

        if (opacity) {
            shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, opacity->GetOpacity());
        } else {
            shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        }
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, viewMatrix);
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, projectionMatrix);
        shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
        vertexBuffer->Render();
        shader->End();
    }

    glEnable(GL_CULL_FACE);
}
