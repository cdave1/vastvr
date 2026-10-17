#include "BlockMover.h"

using namespace hd;

BlockMover::BlockMover(EntityPool *entityPool) : EntitySystem(entityPool), m_musicPlayer(NULL) {}


void BlockMover::Update() {
    EntityQuery *blocks = m_entityPool->Query<physics2d::Body>();
    
	for (int i = 0; i < blocks->Size(); ++i) {
        Entity *block = blocks->At(i);

        const physics2d::Body *body = block->Get<physics2d::Body>();
        graphics::Transform *transform = block->Edit<graphics::Transform>();
        
        Vec3 rotation(0.0f, 0.0f, body->GetAngle());
        Vec3 position = body->GetPosition();
        
        transform->SetOrigin(position);
    }
    
    const std::string BLOCK_NAME = "Block";
    blocks = m_entityPool->QueryByName(BLOCK_NAME);
    
	for (int i = 0; i < blocks->Size(); ++i) {
        Entity *entity = blocks->At(i);
        
        auto transform = entity->Edit<hd::graphics::Transform>();
        auto material = entity->Edit<hd::graphics::Material>();
        
        if (m_musicPlayer && m_musicPlayer->IsActive()) {
            float amplitude = 0.0f;
            if (m_musicPlayer->GetAmplitudeHistory()->Size() > 0) {
                amplitude = m_musicPlayer->GetAmplitudeHistory()->At(i % m_musicPlayer->GetAmplitudeHistory()->Size());

                if (material) {
                    material->SetAmbientAdjustment(Color4f(amplitude, amplitude, amplitude, 0.0f));
                }
            }

            transform->SetScale(hd::clamp(0.8f + (1.05f * amplitude), 0.0f, 1.05f));
        }
    }
}