#include "InputSystem.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;
using namespace hd::application;

InputSystem::InputSystem(EntityPool *entityPool,
                                   Client *client,
                                   const std::string entityName) :
EntitySystem(entityPool),
m_client(client),
m_entityName(entityName) {}


void InputSystem::Update(Entity *currentCamera) {
    hdAssert(m_client);
    hdAssert(m_client->GetEventQueue());

    const hd::Vector<Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();

    for (int i = 0; i < events->Size(); ++i) {
        Entity *event = events->At(i);

        BaseEvent *baseEvent = event->Edit<BaseEvent>();
        hdAssert(baseEvent);
        hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
        if (baseEvent->IsConsumed()) {
            continue;
        }

        if (event->ContainsComponent<pointer::Position, pointer::Move>()) {
            PointerMoveQuery(event, currentCamera);
        } else if (event->ContainsComponent<pointer::Position, pointer::Select>()) {
            PointerSelectQuery(event, currentCamera);
        } else if (event->ContainsComponent<pointer::Position, pointer::Drag>()) {
            PointerDragQuery(event, currentCamera);
        } else if (event->ContainsComponent<pointer::Position, pointer::Release>()) {
            PointerReleaseQuery(event, currentCamera);
        }

        if (event->ContainsComponent<gamepad::ControlSelect>()) {
            if (event->Get<gamepad::ControlSelect>()->GetControl() == gamepad::Control::XBOX_A
                || event->Get<gamepad::ControlSelect>()->GetControl() == gamepad::Control::PLAYSTATION_CROSS) {
                ButtonSelectQuery(event, currentCamera);
            }
        }
        if (event->ContainsComponent<gamepad::ControlRelease>()) {
            if (event->Get<gamepad::ControlRelease>()->GetControl() == gamepad::Control::XBOX_A
                || event->Get<gamepad::ControlRelease>()->GetControl() == gamepad::Control::PLAYSTATION_CROSS) {
                ButtonReleaseQuery(event, currentCamera);
            }
        }

        if (event->ContainsComponent<keyboard::KeySelect>()) {
            auto key = event->Get<keyboard::KeySelect>();
            if (key && key->GetKeyCode() == hd::Device::Keycode::SPACE) {
                ButtonSelectQuery(event, currentCamera);
            }
        }

        if (event->ContainsComponent<keyboard::KeyRelease>()) {
            auto key = event->Get<keyboard::KeyRelease>();
            if (key) {
                if (key->GetKeyCode() == hd::Device::Keycode::SPACE) {
                    ButtonReleaseQuery(event, currentCamera);
                }
            }
        }

        if (!baseEvent->IsConsumed()) {
            LookAtQuery(event, currentCamera);
        }
    }
}


void InputSystem::PointerMoveQuery(Entity *event, Entity *currentCamera) {
    auto pointer = event->Get<pointer::Position>();
    auto move = event->Get<pointer::Move>();

    hdAssert(pointer && move);

    Collision3 collision;
    Entity *selectedBlock = RayTestQuery(event, currentCamera, pointer->GetPosition(currentCamera), &collision);

    if (selectedBlock) {
        auto baseEvent = event->Edit<BaseEvent>();
        auto responderState = selectedBlock->Edit<pointer::ResponderState>();
        auto moveResponder = selectedBlock->Edit<pointer::MoveResponder>();

        if (moveResponder && move->GetControl() == moveResponder->GetControl()
            && responderState->GetCurrentState() == pointer::SelectionState::FREE) {
            moveResponder->Move(selectedBlock, collision, pointer->GetDelta());
            baseEvent->ConsumeEvent(selectedBlock->GetID());
        }
    }
}


void InputSystem::PointerSelectQuery(Entity *event, Entity *currentCamera) {
    auto pointer = event->Get<pointer::Position>();
    auto pointerSelect = event->Get<pointer::Select>();

    hdAssert(pointer && pointerSelect);

    Collision3 collision;
    Entity *selectedBlock = RayTestQuery(event, currentCamera, pointer->GetPosition(currentCamera), &collision);

    if (selectedBlock) {
        auto baseEvent = event->Edit<BaseEvent>();
        auto responderState = selectedBlock->Edit<pointer::ResponderState>();
        auto selectResponder = selectedBlock->Edit<pointer::SelectResponder>();

        if (selectResponder && responderState->GetCurrentState() == pointer::SelectionState::FREE) {
            selectResponder->Select(selectedBlock, collision);
            responderState->Select();
            baseEvent->ConsumeEvent(selectedBlock->GetID());
        }
    }
}


void InputSystem::PointerDragQuery(Entity *event, Entity *currentCamera) {
    auto pointer = event->Get<pointer::Position>();
    auto drag = event->Get<pointer::Drag>();

    Vec3 adjusted = pointer->GetPosition(currentCamera);

    EntityQuery *blocks = m_entityPool->Query<Transform, pointer::ResponderState, pointer::DragResponder>(m_entityName);

    Vec3 currentHitLocation, testRayHitLocation;

    for (int i = 0; i < blocks->Size(); ++i) {
        auto block = blocks->At(i);
        auto baseEvent = event->Edit<BaseEvent>();

        auto collisionData = block->Get<CollisionData>();
        if (collisionData->GetCurrentFrame() != m_client->GetTimer()->GetCurrentFrame()) {
            continue;
        }

        auto responderState    = block->Edit<pointer::ResponderState>();
        auto responder         = block->Edit<pointer::DragResponder>();

        if (responderState->GetCurrentState() == pointer::SelectionState::SELECTED) {
            //if (hd::FastHypotenuse(pointer->GetDelta()[0], pointer->GetDelta()[1]) >= 1.0f) {
                responder->Drag(block, adjusted, pointer->GetDelta());
                responderState->Drag();
            // }
            baseEvent->ConsumeEvent(block->GetID());
            return;
        } else if (responderState->GetCurrentState() == pointer::SelectionState::DRAGGED) {
            responder->Drag(block, adjusted, pointer->GetDelta());
            responderState->Drag();
            baseEvent->ConsumeEvent(block->GetID());
            return;
        }
    }
}


void InputSystem::PointerReleaseQuery(Entity *event, Entity *currentCamera) {
    auto pointer = event->Get<pointer::Position>();
    auto release = event->Get<pointer::Release>();

    hdAssert(pointer && release);

    Vec3 adjusted = pointer->GetPosition(currentCamera);

    EntityQuery *blocks = m_entityPool->Query<Transform, pointer::ResponderState, pointer::ReleaseResponder>(m_entityName);

    auto headset = currentCamera->Get<Headset>();
    auto rayTracer = currentCamera->Get<RayTracer>();

    Vec3 output = rayTracer->PointerToWorld(pointer->GetPosition(),
                                            headset->GetViewport(),
                                            headset->GetProjection(),
                                            headset->GetLookAtMatrix());

    Vec3 rayOrigin = headset->GetWorldPosition();
    Vec3 rayDirection = output - headset->GetWorldPosition();

    for (int i = 0; i < blocks->Size(); ++i) {
        Entity *block = blocks->At(i);
        auto collisionData = block->Get<CollisionData>();

        if (collisionData->GetCurrentFrame() != m_client->GetTimer()->GetCurrentFrame()) {
            continue;
        }

        auto baseEvent = event->Edit<BaseEvent>();
        auto responderState = block->Edit<pointer::ResponderState>();
        auto responder = block->Edit<pointer::ReleaseResponder>();
        auto transform = block->Edit<Transform>();

        if (responderState->GetCurrentState() == pointer::SelectionState::SELECTED ||
            responderState->GetCurrentState() == pointer::SelectionState::DRAGGED) {
            responder->Release(block, adjusted);
            responderState->Release();

            if (block->ContainsComponent<CollisionData>()) {
                auto collisionMesh = block->Get<CollisionData>();

                Collision3 collision;
                if (collisionMesh && RayHitTester::TestRayMeshData(rayOrigin,
                                                                   rayDirection,
                                                                   collisionMesh->GetMeshData(),
                                                                   &collision, transform->GetMatrix())) {
                    responder->ReleaseInside(block, collision);
                } else {
                    responder->ReleaseOutside(block, Collision3());
                }
                baseEvent->ConsumeEvent(block->GetID());
            }
            else if (block->ContainsComponent<hd::graphics::Polygon>()) {
				auto *polygon = block->Get<hd::graphics::Polygon>();

                Collision3 collision;
                if (polygon && RayHitTester::TestRayPolygon(rayOrigin,
                                                            rayDirection, polygon, &collision, transform->GetMatrix())) {
                    responder->ReleaseInside(block, collision);
                } else {
                    responder->ReleaseOutside(block, Collision3());
                }
                baseEvent->ConsumeEvent(block->GetID());
            }
        }
    }
}


void InputSystem::ButtonSelectQuery(hd::Entity *event, hd::Entity *currentCamera) {
    auto pointerSelect = event->Get<pointer::Select>();
    auto headset = currentCamera->Get<Headset>();

    Vec3 position(0.5f * headset->GetViewport().Width(), 0.5f * headset->GetViewport().Height(), 0.0f);

    Collision3 collision;
    Entity *selectedBlock = RayTestQuery(event, currentCamera, position, &collision);

    if (selectedBlock) {
        auto responderState = selectedBlock->Edit<pointer::ResponderState>();
        auto selectResponder = selectedBlock->Edit<pointer::SelectResponder>();
        auto baseEvent = event->Edit<BaseEvent>();

        if (selectResponder && responderState->GetCurrentState() == pointer::SelectionState::FREE) {
            selectResponder->Select(selectedBlock, collision);
            responderState->Select();
            baseEvent->ConsumeEvent(selectedBlock->GetID());
        }
    }
}


void InputSystem::ButtonReleaseQuery(hd::Entity *event, hd::Entity *currentCamera) {
    auto pointerSelect = event->Get<pointer::Select>();
    auto headset = currentCamera->Get<Headset>();

    Vec3 position(0.5f * headset->GetViewport().Width(), 0.5f * headset->GetViewport().Height(), 0.0f);

    Collision3 collision;
    Entity *selectedBlock = RayTestQuery(event, currentCamera, position, &collision);

    EntityQuery *blocks = m_entityPool->Query<Transform, pointer::ResponderState, pointer::ReleaseResponder>(m_entityName);
    for (int i = 0; i < blocks->Size(); ++i) {
        Entity *block = blocks->At(i);
        auto state = block->Edit<pointer::ResponderState>();
        if (state->GetCurrentState() == pointer::SelectionState::SELECTED ||
            state->GetCurrentState() == pointer::SelectionState::DRAGGED) {
            auto release = block->Edit<pointer::ReleaseResponder>();
            state->Release();
            release->Release(block, collision);
            if (block == selectedBlock) release->ReleaseInside(block, collision);
            else release->ReleaseOutside(block, Collision3());
            event->Edit<BaseEvent>()->ConsumeEvent(block->GetID());
        }
    }
}


void InputSystem::GamepadQuery(hd::Entity *event) {
}


// Send an event to what the camera is currently focused on.
// Possible names for the responder:
//      PotentialResponder
//      FocusResponder
//      LookAtResponder
//      HoverResponder
void InputSystem::LookAtQuery(hd::Entity *event, hd::Entity *currentCamera) {
    auto headset = const_cast<Headset *>(currentCamera->Get<Headset>());
    auto rayTracer = currentCamera->Get<RayTracer>();

    EntityQuery *blocks = m_entityPool->Query<CollisionData, Transform, pointer::ResponderState, hd::input::LookAtResponder>(m_entityName);

    Vec3 lookAtPosition(0.5f * headset->GetViewport().Width(), 0.5f * headset->GetViewport().Height(), 0.0f);

    Collision3 collision;
    Entity *selectedBlock = RayTestQuery(event, currentCamera, lookAtPosition, &collision);

    if (selectedBlock) {
        auto baseEvent = event->Edit<BaseEvent>();
        auto responderState = selectedBlock->Edit<pointer::ResponderState>();
        auto responder = selectedBlock->Edit<hd::input::LookAtResponder>();

        if (responderState->GetLookAtState() == pointer::LookAtState::FREE) {
            responderState->LookAt();
            responder->LookAtBegin(selectedBlock, headset, collision);
            //baseEvent->ConsumeEvent(selectedBlock->GetID());

        } else if (responderState->GetLookAtState() == pointer::LookAtState::LOOKAT) {
            responderState->LookAt();
            responder->LookAtHover(selectedBlock, headset, collision);
            //baseEvent->ConsumeEvent(selectedBlock->GetID());
            /*
            if (responderState->GetCurrentState() == pointer::SelectionState::SELECTED) {
                auto dragResponder = selectedBlock->Get<hd::input::pointer::DragResponder>();
                if (dragResponder) {
                    dragResponder->Drag(selectedBlock, collision, Vec3());
                }
            }
             */
        }

        responder->LookAt(selectedBlock, headset, collision);
    }

    for (int i = 0; i < blocks->Size(); ++i) {
        Entity *block = blocks->At(i);
        auto collisionData = block->Get<CollisionData>();

        if (collisionData->GetCurrentFrame() != m_client->GetTimer()->GetCurrentFrame()) {
            continue;
        }

        if (block == selectedBlock) {
            continue;
        }

        auto baseEvent = event->Edit<BaseEvent>();
        auto responderState = block->Edit<pointer::ResponderState>();
        auto responder = block->Edit<hd::input::LookAtResponder>();

        if (responderState->GetLookAtState() == pointer::LookAtState::LOOKAT) {
            responderState->UnLookAt();
            responder->LookAtEnd(block, headset, collision);
            //baseEvent->ConsumeEvent(block->GetID());
        }
    }
}


bool sortRayCollisionResults(hd::Entity *a, hd::Entity *b) {
    auto aa = a->Get<CollisionData>();
    auto bb = b->Get<CollisionData>();

    return aa->GetDistanceToCamera() > bb->GetDistanceToCamera();
}


hd::Entity * InputSystem::RayTestQuery(hd::Entity *event, hd::Entity *currentCamera,
                                       Vec3 pointerPosition,
                                       Collision3 *collision) {
    EntityQuery *entities = m_entityPool->Query<CollisionData, Transform,
                                                pointer::ResponderState,
                                                hd::input::LookAtResponder>(m_entityName);

    auto headset = currentCamera->Get<Headset>();
    auto rayTracer = currentCamera->Get<RayTracer>();

    Vec3 output = rayTracer->PointerToWorld(pointerPosition,
                                            headset->GetViewport(),
                                            headset->GetProjection(),
                                            headset->GetLookAtMatrix());

    Entity *selectedBlock = NULL;
    Vec3 rayOrigin = headset->GetWorldPosition();
    Vec3 rayDirection = output - headset->GetWorldPosition();
    Collision3 selectedBlockCollision, currentCollision;

    hd::Vector<hd::Entity *> fastTestResults(32);

    for (int i = 0; i < entities->Size(); ++i) {
        Entity *block = entities->At(i);
        auto collisionData = block->Get<CollisionData>();

        if (collisionData->GetCurrentFrame() != m_client->GetTimer()->GetCurrentFrame()) {
            continue;
        }

        if (RayHitTester::TestFastRayMeshData(rayOrigin,
                                              rayDirection,
                                              collisionData->GetMeshData(),
                                              &selectedBlockCollision,
                                              collisionData->GetTransformMatrix())) {
            float distance = (rayOrigin - selectedBlockCollision.origin).Length();

            currentCollision = selectedBlockCollision;
            block->Edit<CollisionData>()->SetDistanceToCamera(distance);
            selectedBlock = block;
            fastTestResults.Add(block);
        }
    }

    if (fastTestResults.Size() == 0) {
        return nullptr;
    }

    std::sort(fastTestResults.IteratorBegin(), fastTestResults.IteratorEnd(), sortRayCollisionResults);

    float nearestDistance = FLT_MAX;
    Entity *nearestBlock = nullptr;
    while (!fastTestResults.IsEmpty()) {
        Entity *block = fastTestResults.Pop();

        auto opacity = block->Get<OpacityData>();
        if (opacity && opacity->GetOpacity() == 0.0f) {
            continue;
        }

        if (block->ContainsComponent<CollisionData>()) {
            auto collisionData = block->Get<CollisionData>();

            if (RayHitTester::TestRayMeshData(rayOrigin,
                                              rayDirection,
                                              collisionData->GetMeshData(),
                                              &selectedBlockCollision,
                                              collisionData->GetTransformMatrix())) {
                const float distance = (rayOrigin - selectedBlockCollision.origin).Length();
                if (distance < nearestDistance) {
                    nearestDistance = distance;
                    nearestBlock = block;
                    *collision = selectedBlockCollision;
                }
            }

        }
    }

    return nearestBlock;
}
