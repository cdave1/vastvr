#include "CollisionVolumeRenderer.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

CollisionVolumeRenderer::CollisionVolumeRenderer(EntityPool *entityPool,
                                           hd::application::Client *client,
                                           const std::string entityName) :
    EntitySystem(entityPool),
    m_client(client),
    m_entityName(entityName)
{}


void CollisionVolumeRenderer::Render(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    auto headset = camera->GetComponent<hd::input::Headset>();
    Vec3 cameraPosition = headset->GetEyeWorldPosition(eyePosition);

    EntityQuery *collisionVolumes = m_entityPool->QueryByName(m_entityName);
    hd::Vector<Entity *> opaqueItems;

	for (int i = 0; i < collisionVolumes->Size(); ++i) {
        Entity *entity = collisionVolumes->At(i);

        auto worldTransform = entity->Get<hd::graphics::WorldTransform>();
        auto localTransform = entity->Get<hd::graphics::LocalTransform>();
        auto buffer = entity->Get<graphics::VertexBuffer>();
        auto shader = entity->Get<graphics::Shader>();
        auto mat = entity->Get<graphics::Material>();
        auto collisionVolume = entity->Get<hd::graphics::CollisionVolume>();

        Size2 nodeCount = collisionVolume->GetNodeCount();
        Vec3 nodeSize = collisionVolume->GetNodeSize();

        shader->Begin();
        shader->SetUniform1f(hd::graphics::OPACITY_VALUE_UNIFORM, 1.0f);
        shader->SetUniform3f(hd::graphics::CAMERA_POSITION_UNIFORM, cameraPosition);
        buffer->SetDefaultAttributeLocations(shader);
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

        Matrix worldMatrix = worldTransform->GetMatrix() * localTransform->GetMatrix();

        for (int x = 0; x < nodeCount[0]; ++x) {
            for (int z = 0; z < nodeCount[1]; ++z) {
                Matrix transform;
                Matrix::Translation(transform, x * nodeSize[0], 0.0f, z * nodeSize[2]);

                shader->SetUniformMatrix(hd::graphics::WORLD_MATRIX_UNIFORM, worldMatrix * transform);
                buffer->Render();

                //hd::log::i("%d %d\n", x, z);
            }
        }
        shader->End();
    }
}
