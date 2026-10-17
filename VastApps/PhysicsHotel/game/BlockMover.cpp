#include "BlockMover.h"

using namespace hd;

BlockMover::BlockMover(EntityPool *entityPool) : EntitySystem(entityPool) {}


void BlockMover::Update() {
    EntityQuery *blocks = m_entityPool->Query<physics2d::Body>();
    
	for (int i = 0; i < blocks->Size(); ++i) {
        Entity *block = blocks->At(i);

        physics2d::Body *body                = block->GetComponent<physics2d::Body>();
        graphics::WorldData *worldData = block->GetComponent<graphics::WorldData>();
        graphics::Polygon *polygon = block->GetComponent<graphics::Polygon>();
        
        Vec3 rotation(0.0f, 0.0f, body->GetAngle());
        Vec3 position = body->GetPosition();
        
        worldData->SetOrigin(position);
        worldData->SetRotation(rotation);
    }
}