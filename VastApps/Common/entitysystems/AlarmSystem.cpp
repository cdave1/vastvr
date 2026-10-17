#include "AlarmSystem.h"

using namespace hd;

AlarmSystem::AlarmSystem(EntityPool *entityPool) : EntitySystem(entityPool) {}


void AlarmSystem::Update() {
    EntityQuery *entities = m_entityPool->Query<hd::input::Alarm>();

	for (int i = 0; i < entities->Size(); ++i) {
        Entity *entity = entities->At(i);

        auto alarm = entity->Edit<hd::input::Alarm>();

        if (alarm->IsActive()) {
            alarm->Update(0.016f);
        }
    }
}