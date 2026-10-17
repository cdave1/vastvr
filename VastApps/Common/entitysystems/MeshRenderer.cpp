#include "MeshRenderer.h"
#include <algorithm>

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

MeshRenderer::MeshRenderer(EntityPool *entityPool,
                           hd::application::Client *client,
                           const std::string entityName) :
EntitySystem(entityPool),
m_client(client),
m_entityName(entityName),
m_queryList(entityPool->GetCapacity())
{}


static Vec3 cameraPosition;

bool sort (hd::Entity *a, hd::Entity *b) {
    auto xformA = a->Get<Transform>();
    auto xformB = b->Get<Transform>();

    return (cameraPosition - xformA->GetOrigin()).Length() < (cameraPosition - xformB->GetOrigin()).Length();
}


void MeshRenderer::Update(hd::Entity *camera) {
    EntityQuery *blocks = m_entityPool->Query<graphics::CollisionData, graphics::Material>(m_entityName);

    auto headset = camera->GetComponent<hd::input::Headset>();
    Matrix viewMatrix = headset->GetLookAtMatrix(hd::input::EyePosition::Right);
    Matrix projectionMatrix = headset->GetProjection(hd::input::EyePosition::Right);

    Matrix mvp = projectionMatrix * viewMatrix;
    Frustum frustum(mvp);

    m_queryList.Clear();

    for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);

        auto transform = entity->GetComponent<hd::graphics::Transform>();
        auto mesh = entity->GetComponent<hd::graphics::MeshReference>();
        auto collisionData = entity->GetComponent<CollisionData>();

        m_queryList.Add(entity);
        /*
        if (collisionData) {
            if (collisionData->GetCurrentFrame() == m_client->GetTimer()->GetCurrentFrame()) {
                m_queryList.Add(entity);
            }
        } else {
            if (mesh) {
                if (frustum.AABBInside(transform->GetOrigin(), transform->GetScale(), mesh->GetMeshData()->GetAABB())) {
                    m_queryList.Add(entity);
                }
            } else {
                m_queryList.Add(entity);
            }
        }*/
    }

    // Sort from closest to farthest.
    cameraPosition = headset->GetWorldPosition();
   // std::sort(m_queryList.First(), m_queryList.Last(), sort);
}


void MeshRenderer::RenderOpaque(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();

    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
    Matrix viewMatrix = headset->GetLookAtMatrix(eyePosition);
    Matrix projectionMatrix = headset->GetProjection(eyePosition);


    //glDisable(GL_DEPTH_TEST);
    for (int i = 0; i < m_queryList.Size(); ++i) {
        Entity *entity = m_queryList.At(i);

        auto opacityData = entity->GetComponent<hd::graphics::OpacityData>();

        if (opacityData && opacityData->GetOpacity() < 1.0) {
            continue;
        }

        auto transform = entity->GetComponent<hd::graphics::Transform>();
        auto mesh = entity->GetComponent<graphics::MeshReference>();
        auto material = entity->GetComponent<graphics::Material>();

        auto shader = entity->Edit<graphics::Shader>();
        shader->Begin();
        shader->BindTexture("iTexture0", material->GetTexture());
        shader->BindTexture("iNormalTexture0", material->GetNormalTexture());

        shader->SetUniform4f(hd::graphics::MATERIAL_AMBIENT_COLOR_UNIFORM, material->GetAmbient());
        shader->SetUniform4f(hd::graphics::MATERIAL_DIFFUSE_COLOR_UNIFORM, material->GetDiffuse());
        shader->SetUniform4f(hd::graphics::MATERIAL_SPECULAR_COLOR_UNIFORM, material->GetSpecular());
        shader->SetUniform1f(hd::graphics::MATERIAL_SHININESS_UNIFORM, material->GetShininess());

        for (int j = 0; j < lights->Size(); ++j) {
            Entity *lightEntity = lights->At(0);
            auto light = lightEntity->Get<graphics::Light>();
            LightRenderer::SetLightUniforms(light, shader);
        }


        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, viewMatrix);
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, projectionMatrix);
        shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
        mesh->Render();
        shader->End();
    }
    //glEnable(GL_DEPTH_TEST);
}


void MeshRenderer::RenderTransparent(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();

    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
    Matrix viewMatrix = headset->GetLookAtMatrix(eyePosition);
    Matrix projectionMatrix = headset->GetProjection(eyePosition);

    for (int i = 0; i < m_queryList.Size(); ++i) {
        Entity *entity = m_queryList.At(i);

        auto opacityData = entity->GetComponent<hd::graphics::OpacityData>();

        if (!opacityData || opacityData->GetOpacity() == 1.0 || opacityData->GetOpacity() == 0.0) {
            continue;
        }

        auto transform = entity->GetComponent<hd::graphics::Transform>();
        auto mesh = entity->GetComponent<graphics::MeshReference>();
        auto mat = entity->GetComponent<graphics::Material>();

        auto shader = entity->Edit<graphics::Shader>();

        shader->Begin();
        if (mat->GetTexture()) {
            shader->BindTexture("iTexture0", mat->GetTexture());
        }
        if (mat->GetNormalTexture()) {
            shader->BindTexture("iNormalTexture0", mat->GetNormalTexture());
        }

        LightRenderer::SetMaterialUniforms(mat, shader);

        for (int j = 0; j < lights->Size(); ++j) {
            Entity *lightEntity = lights->At(0);
            auto light = lightEntity->Get<graphics::Light>();
            LightRenderer::SetLightUniforms(light, shader);
        }

        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, opacityData->GetOpacity());
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, viewMatrix);
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, projectionMatrix);
        shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
        mesh->Render();
        shader->End();
    }
}


void MeshRenderer::RenderLines(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    EntityQuery *items = m_entityPool->Query<graphics::Shader, graphics::MeshReference, graphics::Material>(m_entityName);

    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
    Matrix viewMatrix = headset->GetLookAtMatrix(eyePosition);
    Matrix projectionMatrix = headset->GetProjection(eyePosition);

    for (int i = 0; i < items->Size(); ++i) {
        Entity *entity = items->At(i);

        auto transform = entity->GetComponent<hd::graphics::Transform>();
        auto mesh = entity->GetComponent<graphics::MeshReference>();
        auto shader = entity->Edit<graphics::Shader>();
        auto mat = entity->GetComponent<graphics::Material>();

        shader->Begin();
        if (mat->GetTexture()) {
            shader->BindTexture("iTexture0", mat->GetTexture());
        }
        if (mat->GetNormalTexture()) {
            shader->BindTexture("iNormalTexture0", mat->GetNormalTexture());
        }

        LightRenderer::SetMaterialUniforms(mat, shader);

        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, viewMatrix);
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, projectionMatrix);
        shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
        mesh->RenderLines();
        shader->End();
    }
}
