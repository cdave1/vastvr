#include "BlockInputSystem.h"
#include "Game.h"

using namespace hd;
using namespace hd::base;
using namespace hd::input;
using namespace hd::graphics;
using namespace hd::application;

BlockInputSystem::BlockInputSystem(EntityPool *entityPool,
                                   Client *client) :
    EntitySystem(entityPool),
    m_client(client),
    m_game(NULL) {}


void BlockInputSystem::Update(Entity *currentCamera) {
    hdAssert(m_client);
    hdAssert(m_client->GetEventQueue());
    
    const hd::Vector<Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();
    
    for (int i = 0; i < events->Size(); ++i) {
        Entity *event = events->At(i);
        
        BaseEvent *baseEvent = event->GetComponent<BaseEvent>();
        hdAssert(baseEvent);
        hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
        if (baseEvent->IsConsumed()) {
            continue;
        }
        
        if (event->ContainsComponent<pointer::Position, pointer::Select>()) {
            PointerSelectQuery(event, currentCamera);
        } else if (event->ContainsComponent<pointer::Position, pointer::Drag>()) {
            PointerDragQuery(event, currentCamera);
        } else if (event->ContainsComponent<pointer::Position, pointer::Release>()) {
            PointerReleaseQuery(event, currentCamera);
        }
    }
}


void BlockInputSystem::PointerSelectQuery(Entity *event, Entity *currentCamera) {
    auto baseEvent = event->GetComponent<BaseEvent>();
    auto pointer = event->GetComponent<pointer::Position>();
    auto pointerSelect = event->GetComponent<pointer::Select>();
    
    hdAssert(pointer && pointerSelect);
    
    EntityQuery *entities = m_entityPool->Query<WorldData, pointer::ResponderState, pointer::SelectResponder, Polygon>();
	for (int i = 0; i < entities->Size(); ++i) {
        Entity *entity = entities->At(i);
        
        auto *responderState = entity->GetComponent<pointer::ResponderState>();
        
        if (responderState->GetCurrentState() == pointer::FREE) {
            auto responder = entity->GetComponent<pointer::SelectResponder>();
            auto polygon = entity->GetComponent<Polygon>();
            auto worldData = entity->GetComponent<WorldData>();
            
            auto cameraData = currentCamera->GetComponent<CameraData>();
            auto perspective = currentCamera->GetComponent<PerspectiveData>();
            auto screen = currentCamera->GetComponent<ScreenData>();
            auto rayTracer = currentCamera->GetComponent<RayTracer>();
            auto raytraceLines = currentCamera->GetComponent<Polygon>();

            baseEvent->ConsumeEvent(currentCamera->GetID());
            /*
             Vec3 delta = pointer->GetDelta(currentCamera);
             Vec3 diff = cameraData->GetTarget() - cameraData->GetLocation();
             Vec3 n = hd::normalize(diff);
             
             delta *= 0.0025f; // assume this is the angle in radians
             
             // We want to rotate the current target around the y axis by the x
             // distance of this delta
             Matrix xyz;
             Vec3 target;
             Matrix::RotationXYZOrigin(xyz, Vec3(5.0f * delta[1] * hd::abs(n[0]), delta[0], delta[1] * hd::abs(n[2])));
             Matrix::Vec3Multiply(target, diff, xyz); */
            
            Vec3 output = rayTracer->PointerToWorld(pointer->GetPosition(),
                                                    screen->GetAABB(),
                                                    perspective,
                                                    cameraData);
            
            raytraceLines->AddLine(cameraData->GetLocation(), output);
        
            if (polygon && PolygonHitTester::TestRay(output, cameraData->GetLocation(), polygon, worldData)) {
                responderState->Select();
                baseEvent->ConsumeEvent(entity->GetID());
                return;
            }
        }
    }
}


void BlockInputSystem::PointerDragQuery(Entity *event, Entity *currentCamera) {
    auto baseEvent = event->GetComponent<BaseEvent>();
    auto pointer = event->GetComponent<pointer::Position>();
    auto drag = event->GetComponent<pointer::Drag>();
    
    Vec3 adjusted = pointer->GetPosition(currentCamera);
    
    EntityQuery *blocks = m_entityPool->Query<WorldData, pointer::ResponderState, pointer::DragResponder, Polygon>();
    
    for (int i = 0; i < blocks->Size(); ++i) {
        Entity *block = blocks->At(i);
        auto responderState    = block->GetComponent<pointer::ResponderState>();
        auto responder         = block->GetComponent<pointer::ReleaseResponder>();
        
        if (responderState->GetCurrentState() == pointer::SELECTED) {
            if (hd::FastHypotenuse(pointer->GetDelta()[0], pointer->GetDelta()[1]) >= 1.0f) {
                m_game->OnBlockDrag(block, adjusted, pointer->GetDelta());
                responderState->Drag();
            }
            baseEvent->ConsumeEvent(block->GetID());
            return;
        } else if (responderState->GetCurrentState() == pointer::DRAGGED) {
            m_game->OnBlockDrag(block, adjusted, pointer->GetDelta());
            responderState->Drag();
            baseEvent->ConsumeEvent(block->GetID());
            return;
        }
    }
}


void BlockInputSystem::PointerReleaseQuery(Entity *event, Entity *currentCamera) {
    auto *baseEvent = event->GetComponent<BaseEvent>();
    auto *pointer = event->GetComponent<pointer::Position>();
    auto *release = event->GetComponent<pointer::Release>();
    
    hdAssert(pointer && release);
    
    Vec3 adjusted = pointer->GetPosition(currentCamera);
    
    EntityQuery *blocks = m_entityPool->Query<WorldData, pointer::ResponderState, pointer::ReleaseResponder, Polygon>();

	for (int i = 0; i < blocks->Size(); ++i) {
        Entity *block = blocks->At(i);
        auto responderState = block->GetComponent<pointer::ResponderState>();
        auto responder = block->GetComponent<pointer::ReleaseResponder>();
        auto worldData = block->GetComponent<hd::graphics::WorldData>();
        
        auto cameraData = currentCamera->GetComponent<CameraData>();
        auto perspective = currentCamera->GetComponent<PerspectiveData>();
        auto screen = currentCamera->GetComponent<ScreenData>();
        auto rayTracer = currentCamera->GetComponent<RayTracer>();
        
        baseEvent->ConsumeEvent(currentCamera->GetID());
        
        Vec3 output = rayTracer->PointerToWorld(pointer->GetPosition(),
                                                screen->GetAABB(),
                                                perspective,
                                                cameraData);
        
        if (responderState->GetCurrentState() == pointer::SELECTED) {
            auto *polygon = block->GetComponent<Polygon>();
            
            if (polygon && PolygonHitTester::TestRay(output, cameraData->GetLocation(), polygon, worldData)) {
                m_game->OnBlockRelease(block, adjusted);
                responderState->Release();
                baseEvent->ConsumeEvent(block->GetID());
                return;
            }
        } else if (responderState->GetCurrentState() == pointer::DRAGGED) {
            m_game->OnBlockRelease(block, adjusted);
            responderState->Release();
            baseEvent->ConsumeEvent(block->GetID());
            return;
        }
    }
}
