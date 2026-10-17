#include "InstancedRenderer.h"
#include <algorithm>

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

static const uint32_t MAX_MESH_INSTANCE_SIZE = 4096;
static MeshInstanceData meshInstanceData[MAX_MESH_INSTANCE_SIZE];
static Shader *depthShader = nullptr;

InstancedRenderer::InstancedRenderer(EntityPool *entityPool,
                                           hd::application::Client *client,
                                           const std::string entityName) :
    EntitySystem(entityPool),
    m_client(client),
    m_entityName(entityName),
    m_queryList(entityPool->GetCapacity()),
    m_meshIndex(entityPool->GetCapacity())
{

    depthShader = Components::Get<Shader>();
    depthShader->LoadShader("depthPass.vert", "depthPass.frag");
}


bool sortInstanced(hd::Entity *a, hd::Entity *b) {
    auto aa = a->Get<CollisionData>();
    auto bb = b->Get<CollisionData>();

    return aa->GetDistanceToCamera() < bb->GetDistanceToCamera();
}


void InstancedRenderer::Update(hd::Entity *camera) {
    EntityQuery *blocks = m_entityPool->Query<CollisionData, Shader, MeshReference, Material>(m_entityName);

    auto headset = camera->GetComponent<hd::input::Headset>();
    Matrix viewMatrix = headset->GetLookAtMatrix(hd::input::EyePosition::Center);
    Matrix projectionMatrix = headset->GetProjection(hd::input::EyePosition::Center);

    m_queryList.Clear();
    m_meshIndex.Clear();

    for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);

        auto transform = entity->GetComponent<Transform>();
        auto collisionData = entity->Get<CollisionData>();
        auto material = entity->Get<Material>();

        auto shader = entity->Edit<Shader>();
        auto mesh = entity->Edit<MeshReference>();

        if (collisionData->GetCurrentFrame() == m_client->GetTimer()->GetCurrentFrame()) {
            m_queryList.Add(entity);

            bool paired = false;
            for (int j = 0; j < m_meshIndex.Size(); ++j) {
                MeshShaderPair pair = m_meshIndex.At(j);
                if (pair.shader->GetProgram() == shader->GetProgram() && pair.mesh->GetMeshData() == mesh->GetMeshData()) {
                    paired = true;
                }
            }

            if (!paired) {
                MeshShaderPair newPair;
                newPair.shader = shader;
                newPair.mesh = mesh;
                newPair.texture = nullptr;
                
                if (material && material->GetTexture()) {
                    //newPair.texture = const_cast<hd::Texture *>(material->GetTexture());
                } else {
                    //newPair.texture = nullptr;
                }
                m_meshIndex.Add(newPair);
            }
        }
    }

    std::sort(m_queryList.IteratorBegin(), m_queryList.IteratorEnd(), sortInstanced);
}


static Matrix meshTransforms[MAX_MESH_INSTANCE_SIZE];

void InstancedRenderer::RenderDepth(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
    Matrix viewMatrix = headset->GetLookAtMatrix(eyePosition);
    Matrix projectionMatrix = headset->GetProjection(eyePosition);

    Matrix cameraMatrices[2];
    cameraMatrices[0] = projectionMatrix;
    cameraMatrices[1] = viewMatrix;

    uint32_t meshInstanceCount = 0;
    for (uint32_t i = 0; i < m_meshIndex.Size(); ++i) {
        MeshShaderPair pair = m_meshIndex.At(i);

        auto mesh = pair.mesh;

        meshInstanceCount = 0;
        for (uint32_t j = 0; j < m_queryList.Size(); ++j) {
            auto entity = m_queryList.At(j);

            if (entity->Get<hd::graphics::MeshReference>()->GetMeshData() == mesh->GetMeshData()) {
                auto transform = entity->Get<hd::graphics::Transform>();
                meshTransforms[meshInstanceCount] = transform->GetMatrix();
                meshInstanceCount++;
            }
        }

        mesh->RenderDepthInstanced(cameraMatrices, meshTransforms, meshInstanceCount);
    }
}


void InstancedRenderer::RenderOpaque(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();

    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
    Matrix viewMatrix = headset->GetLookAtMatrix(eyePosition);
    Matrix projectionMatrix = headset->GetProjection(eyePosition);

    uint32_t meshInstanceCount = 0;
    for (int i = 0; i < m_meshIndex.Size(); ++i) {
        MeshShaderPair pair = m_meshIndex.At(i);

        auto shader = pair.shader;
        auto mesh = pair.mesh;
        auto texture = pair.texture;

        meshInstanceCount = 0;
        for (int j = 0; j < m_queryList.Size(); ++j) {
            auto entity = m_queryList.At(j);
            auto opacityData = entity->GetComponent<hd::graphics::OpacityData>();
            if (opacityData && opacityData->GetOpacity() < 1.0) {
                continue;
            }

            if (entity->Get<hd::graphics::MeshReference>()->GetMeshData() == mesh->GetMeshData() &&
                entity->Get<hd::graphics::Shader>()->GetProgram() == shader->GetProgram()) {
                auto transform = entity->Get<hd::graphics::Transform>();
                auto material = entity->Get<hd::graphics::Material>();

                meshInstanceData[meshInstanceCount].matrix = transform->GetMatrix();
                meshInstanceData[meshInstanceCount].materialAmbientColor.Set(material->GetAmbient());
                meshInstanceData[meshInstanceCount].materialDiffuseColor.Set(material->GetDiffuse());
                meshInstanceData[meshInstanceCount].materialSpecularColor.Set(material->GetSpecular());
                meshInstanceData[meshInstanceCount].materialShininess = material->GetShininess();
                meshInstanceCount++;
            }
        }

        shader->Begin();
        if (texture) {
            shader->BindTexture("iTexture0", texture);
        }
        //shader->BindTexture("iTexture0", material->GetTexture());
        //shader->BindTexture("iNormalTexture0", material->GetNormalTexture());
        //mesh->GetMeshData()->SetInstancedAttributeLocations(shader);
        //mesh->GetMeshData()->SetDefaultAttributeLocations(shader);
        
        for (int j = 0; j < lights->Size(); ++j) {
            Entity *lightEntity = lights->At(0);
            auto light = lightEntity->Get<graphics::Light>();
            LightRenderer::SetLightUniforms(light, shader);

            Vec4 lightPosition(light->GetPosition()[0],
                               light->GetPosition()[1],
                               light->GetPosition()[2],
                               light->IsDirectional() ? 1.0f : 0.0f);
            //    Color4f ambient = light->GetAmbient();
            //    Color4f diffuse = light->GetDiffuse();
            //    Color4f specular = light->GetSpecular();

            Matrix::Vec4Multiply(lightPosition, lightPosition, viewMatrix);
            shader->SetUniform4f(hd::graphics::LIGHT_POSITION_UNIFORM, lightPosition);
        }



        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, viewMatrix);
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, projectionMatrix);
        mesh->RenderInstanced(meshInstanceData, meshInstanceCount);
        shader->End();
    }
}


void InstancedRenderer::RenderTransparent(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    EntityQuery *lights = m_entityPool->Query<hd::graphics::Light>();

    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
    Matrix viewMatrix = headset->GetLookAtMatrix(eyePosition);
    Matrix projectionMatrix = headset->GetProjection(eyePosition);

    uint32_t meshInstanceCount = 0;
    for (int i = 0; i < m_meshIndex.Size(); ++i) {
        MeshShaderPair pair = m_meshIndex.At(i);

        auto shader = pair.shader;
        auto mesh = pair.mesh;
        auto texture = pair.texture;

        meshInstanceCount = 0;
        for (int j = 0; j < m_queryList.Size(); ++j) {
            auto entity = m_queryList.At(j);
            auto opacityData = entity->GetComponent<hd::graphics::OpacityData>();
            if (!opacityData || opacityData->GetOpacity() == 1.0 || opacityData->GetOpacity() == 0.0) {
                continue;
            }

            if (entity->Get<hd::graphics::MeshReference>()->GetMeshData() == mesh->GetMeshData() &&
                entity->Get<hd::graphics::Shader>()->GetProgram() == shader->GetProgram()) {
                auto transform = entity->Get<hd::graphics::Transform>();
                auto material = entity->Get<hd::graphics::Material>();

                meshInstanceData[meshInstanceCount].matrix = transform->GetMatrix();
                meshInstanceData[meshInstanceCount].materialAmbientColor.Set(material->GetAmbient());
                meshInstanceData[meshInstanceCount].materialDiffuseColor.Set(material->GetDiffuse());
                meshInstanceData[meshInstanceCount].materialSpecularColor.Set(material->GetSpecular());
                meshInstanceData[meshInstanceCount].materialShininess = material->GetShininess();
                meshInstanceCount++;
            }
        }
        
        shader->Begin();
        if (texture) {
            shader->BindTexture("iTexture0", texture);
        }
        //shader->BindTexture("iNormalTexture0", material->GetNormalTexture());

        for (int j = 0; j < lights->Size(); ++j) {
            Entity *lightEntity = lights->At(0);
            auto light = lightEntity->Get<graphics::Light>();
            LightRenderer::SetLightUniforms(light, shader);
        }

        //opacityData->GetOpacity()
        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, viewMatrix);
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, projectionMatrix);
        mesh->RenderInstanced(meshInstanceData, meshInstanceCount);
        shader->End();
    }
}


void InstancedRenderer::RenderLines(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    EntityQuery *items = m_entityPool->Query<graphics::Shader, graphics::MeshReference, graphics::Material>(m_entityName);

    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 eyeWorldPosition = headset->GetEyeWorldPosition(eyePosition);
    Matrix viewMatrix = headset->GetLookAtMatrix(eyePosition);
    Matrix projectionMatrix = headset->GetProjection(eyePosition);

    for (int i = 0; i < items->Size(); ++i) {
        Entity *entity = items->At(i);

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

        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, eyeWorldPosition);
        shader->SetUniformMatrix(hd::graphics::VIEW_MATRIX_UNIFORM, viewMatrix);
        shader->SetUniformMatrix(hd::graphics::PROJECTION_MATRIX_UNIFORM, projectionMatrix);
        shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, transform->GetMatrix());
        mesh->RenderLines();
        shader->End();
    }
}
