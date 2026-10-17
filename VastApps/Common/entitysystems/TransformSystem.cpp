#include "TransformSystem.h"

using namespace hd;

TransformSystem::TransformSystem(EntityPool *entityPool) : EntitySystem(entityPool) {}


void TransformSystem::Update() {
    {
        EntityQuery *transformEntities = m_entityPool->Query<hd::graphics::Transform>();
        for (int i = 0; i < transformEntities->Size(); ++i) {
            Entity *entity = transformEntities->At(i);
            auto transform = entity->Edit<hd::graphics::Transform>();
            auto parentComponent = entity->Get<hd::data::Parent>();
            if (parentComponent) {
                Entity *parentEntity = parentComponent->GetEntity();
                auto parentTransform = parentEntity->Get<hd::graphics::Transform>();
                if (parentTransform) {
                    transform->SetContext(parentTransform->GetMatrix());
                }
            }
        }
        for (int i = 0; i < transformEntities->Size(); ++i) {
            Entity *entity = transformEntities->At(i);
            auto transform = entity->Edit<hd::graphics::Transform>();
            transform->Update();
        }
    }

    {
        EntityQuery *opacityEntities = m_entityPool->Query<hd::graphics::OpacityData>();
        for (int i = 0; i < opacityEntities->Size(); ++i) {
            Entity *entity = opacityEntities->At(i);
            auto opacity = entity->Edit<hd::graphics::OpacityData>();
            auto parentComponent = entity->Get<hd::data::Parent>();
            if (parentComponent) {
                Entity *parentEntity = parentComponent->GetEntity();
                if (parentEntity) {
                    auto parentOpacity = parentEntity->Get<hd::graphics::OpacityData>();
                    if (parentOpacity) {
                        opacity->SetOpacityRange(parentOpacity->GetOpacity());
                    }
                }
            }
        }
    }
}