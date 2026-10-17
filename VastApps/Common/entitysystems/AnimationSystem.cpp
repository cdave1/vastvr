#include "AnimationSystem.h"

using namespace hd;

AnimationSystem::AnimationSystem(EntityPool *entityPool) : EntitySystem(entityPool) {}


void AnimationSystem::Update() {
    EntityQuery *entities = m_entityPool->Query<hd::animation::AnimationTimer,
                                                hd::graphics::Transform>();
    
	for (int i = 0; i < entities->Size(); ++i) {
        Entity *entity = entities->At(i);

        auto timer = entity->Edit<hd::animation::AnimationTimer>();

        auto transform = entity->Edit<hd::graphics::Transform>();
        auto opacity = entity->Edit<hd::graphics::OpacityData>();

        auto positionBlock = entity->Get<hd::animation::PositionBlock>();
        auto opacityBlock = entity->Get<hd::animation::OpacityBlock>();
        auto scaleBlock = entity->Get<hd::animation::ScaleBlock>();

        if (timer->IsActive()) {
            timer->Update(0.016f);

            if (transform && positionBlock && positionBlock->IsEnabled()) {
                transform->SetOrigin(positionBlock->GetPosition(timer->GetTime()));
            }

            if (transform && scaleBlock && scaleBlock->IsEnabled()) {
                transform->SetScale(scaleBlock->GetScale(timer->GetTime()));
            }

            if (opacity && opacityBlock && opacityBlock->IsEnabled()) {
                opacity->SetOpacity(opacityBlock->GetOpacity(timer->GetTime()));
            }
        }
    }
}