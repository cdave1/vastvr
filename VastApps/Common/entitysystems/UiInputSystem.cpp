#include "UiInputSystem.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;
using namespace hd::application;

UiInputSystem::UiInputSystem(EntityPool *entityPool,
                             Client *client) :
    EntitySystem(entityPool),
    m_client(client) {}


void UiInputSystem::Update() {
    hdAssert(m_client);
    hdAssert(m_client->GetEventQueue());
    
    const hd::Vector<Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();
    
    for (int i = 0; i < events->Size(); ++i) {
        Entity *event = events->At(i);
        
        auto baseEvent = event->Get<BaseEvent>();
        hdAssert(baseEvent);
        hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
        if (baseEvent->IsConsumed()) {
            continue;
        }
        
        if (event->ContainsComponent<pointer::Position, pointer::Select>()) {
            PointerSelectQuery(event);
        } else if (event->ContainsComponent<pointer::Position, pointer::Drag>()) {
            PointerDragQuery(event);
        } else if (event->ContainsComponent<pointer::Position, pointer::Release>()) {
            PointerReleaseQuery(event);
        }
    }
}


void UiInputSystem::PointerSelectQuery(Entity *event) {
    auto baseEvent = event->Edit<BaseEvent>();
    auto pointer = event->GetComponent<pointer::Position>();
    auto pointerSelect = event->GetComponent<pointer::Select>();
    
    hdAssert(pointer && pointerSelect);
    
	hd::EntityQuery *entities = m_entityPool->Query<pointer::ResponderState, pointer::SelectResponder, hd::graphics::Polygon, ScreenData, Transform>();
	for (int i = 0; i < entities->Size(); ++i) {
        Entity *entity = entities->At(i);
        
        Vec3 position = pointer->GetPosition(entity);
        
        auto responderState = entity->Edit<pointer::ResponderState>();
        auto responder = entity->Edit<pointer::SelectResponder>();
        
        if (responderState->GetCurrentState() == pointer::SelectionState::FREE) {
            auto polygon = entity->GetComponent<hd::graphics::Polygon>();
            auto transform = entity->GetComponent<Transform>();
            
            if (polygon && RayHitTester::TestPoint(position, polygon, transform->GetMatrix())) {
                responder->Select(entity, position);
                responderState->Select();
                baseEvent->ConsumeEvent(entity->GetID());
                return;
            }
        }
    }
}


void UiInputSystem::PointerDragQuery(Entity *event)
{
    /*
    auto *baseEvent = event->GetComponent<BaseEvent>();
    auto *pointer = event->GetComponent<pointer::Position>();
    auto *drag = event->GetComponent<pointer::Drag>();
    
    Vec3 adjusted = pointer->GetPosition(currentCamera);
    
    EntityQuery *blocks = m_entityPool->Query<pointer::ResponderState, pointer::DragResponder, Polygon>();
    
    for (int i = 0; i < blocks->Size(); ++i)
	{
        Entity *block = blocks->At(i);
        auto responderState    = block->GetComponent<pointer::ResponderState>();
        auto responder         = block->GetComponent<pointer::ReleaseResponder>();
        
        if (responderState->GetCurrentState() == pointer::SELECTED)
        {
            if (hdFastHypotenuse(pointer->GetDelta().x, pointer->GetDelta().y) >= 1.0f)
            {
                m_game->OnBlockDrag(block, adjusted, pointer->GetDelta());
                responderState->Drag();
            }
            baseEvent->ConsumeEvent(block->GetID());
            return;
        }
        else if (responderState->GetCurrentState() == pointer::DRAGGED)
        {
            m_game->OnBlockDrag(block, adjusted, pointer->GetDelta());
            responderState->Drag();
            baseEvent->ConsumeEvent(block->GetID());
            return;
        }
    }*/
}


void UiInputSystem::PointerReleaseQuery(Entity *event) {
    auto baseEvent = event->Edit<BaseEvent>();
    auto pointer = event->GetComponent<pointer::Position>();
    auto release = event->GetComponent<pointer::Release>();
    
    hdAssert(pointer && release);
    
    hd::EntityQuery *entities = m_entityPool->Query<pointer::ResponderState, pointer::ReleaseResponder, hd::graphics::Polygon, ScreenData, Transform>();
	for (int i = 0; i < entities->Size(); ++i) {
        Entity *entity = entities->At(i);
        
        Vec3 position = pointer->GetPosition(entity);
        
        auto responderState = entity->Edit<pointer::ResponderState>();
        
        if (responderState->GetCurrentState() == pointer::SelectionState::SELECTED) {
            responderState->Release();
            
            auto polygon = entity->Get<hd::graphics::Polygon>();
            auto transform = entity->Get<Transform>();
            auto responder = entity->Edit<pointer::ReleaseResponder>();
            
            if (polygon && RayHitTester::TestPoint(position, polygon, transform->GetMatrix())) {
                responder->Release(entity, position);
                baseEvent->ConsumeEvent(entity->GetID());
                return;
            }
        }
        /*
        else if (responderState->GetCurrentState() == pointer::DRAGGED)
        {
            m_game->OnBlockRelease(block, adjusted);
            responderState->Release();
            baseEvent->ConsumeEvent(block->GetID());
            return;
        }*/
    }
}
