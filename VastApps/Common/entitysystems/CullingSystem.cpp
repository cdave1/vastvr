#include "CullingSystem.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;

CullingSystem::CullingSystem(EntityPool *entityPool, hd::application::Client *client) :
    EntitySystem(entityPool),
    m_client(client)
{}


void CullingSystem::Update(hd::Entity *camera) {
    EntityQuery *entities = m_entityPool->Query<CollisionData, Transform>();

    auto headset = camera->GetComponent<hd::input::Headset>();
    const auto eye = headset->IsStereo() ? hd::input::EyePosition::Right : hd::input::EyePosition::Center;
    Matrix viewMatrix = headset->GetLookAtMatrix(eye);
    Matrix projectionMatrix = headset->GetProjection(eye);

    Matrix mvp = projectionMatrix * viewMatrix;
    Frustum frustum(mvp);

    Vec3 cameraPosition = headset->GetWorldPosition();

    for (int i = 0; i < entities->Size(); ++i) {
        Entity *entity = entities->At(i);

        auto transform = entity->Get<hd::graphics::Transform>();
        auto mesh = entity->Get<hd::graphics::MeshReference>();
        
        auto collisionData = entity->Edit<CollisionData>();

        if (mesh) {
            if (frustum.AABBInside(transform->GetOrigin(), transform->GetScale(), mesh->GetMeshData()->GetAABB())) {
                collisionData->SetCurrentFrame(m_client->GetTimer()->GetCurrentFrame());
                collisionData->SetDistanceToCamera((cameraPosition - transform->GetOrigin()).Length());
            }
        } else {
            collisionData->SetCurrentFrame(m_client->GetTimer()->GetCurrentFrame());
            collisionData->SetDistanceToCamera((cameraPosition - transform->GetOrigin()).Length());
        }
    }
}
