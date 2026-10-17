#include "Cameras.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;
using namespace hd::application;

hd::Entity * Cameras::mainCamera = nullptr;


Cameras::Cameras(EntityPool *entityPool,
                 hd::application::Client *client) : EntitySystem(entityPool),
m_client(client) {}


void Cameras::Update() {
    auto headset = mainCamera->Edit<Headset>();
    hd::input::HMD *hmd = m_client->GetEventQueue()->GetCurrentHMD();

    //headset->SetOrientation(hmd->GetDeviceOrientation());
    headset->SetTrackingOffset(hmd->GetDeviceTrackingOffset());

	headset->SetEyeOffset(hmd->GetEyeOffset(hd::input::EyePosition::Left), hd::input::EyePosition::Left);
	headset->SetEyeOffset(hmd->GetEyeOffset(hd::input::EyePosition::Right), hd::input::EyePosition::Right);

	headset->SetEyeOrientation(hmd->GetEyeOrientation(hd::input::EyePosition::Left),
		hmd->GetEyeOrientation(hd::input::EyePosition::Right));
}


void Cameras::HandleInput() {
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

        if (event->ContainsComponent<pointer::Position, pointer::Move>()) {
            PointerMoveQuery(event);
        } else if (event->ContainsComponent<pointer::Position, pointer::Select>()) {
            PointerSelectQuery(event);
        } else if (event->ContainsComponent<pointer::Position, pointer::Drag>()) {
            PointerDragQuery(event);
        } else if (event->ContainsComponent<pointer::Position, pointer::Release>()) {
            PointerReleaseQuery(event);
        } else if (event->ContainsComponent<keyboard::KeySelect>()) {
            //ButtonDownQuery(event, currentCamera);
        } else if (event->ContainsComponent<keyboard::KeyRelease>()) {
            //ButtonUpQuery(event, currentCamera);
        }

        if (event->ContainsComponent<device::Keyboard>()) {
            KeyboardQuery(event);
        }
        if (event->ContainsComponent<device::Gamepad>()) {
            GamepadQuery(event);
        }
    }
}

static Quaternion currentXRotation;
static Quaternion currentYRotation;
static Quaternion currentZRotation;
static Vec3 currentDelta;

void Cameras::PointerMoveQuery(Entity *event) {
    auto pointer = event->Get<pointer::Position>();
    auto rayTracer = mainCamera->Get<RayTracer>();

    auto responderState = mainCamera->Edit<pointer::ResponderState>();
    auto headset = mainCamera->Edit<Headset>();

    Vec3 output = rayTracer->PointerToWorld(pointer->GetPosition(),
                                            headset->GetViewport(),
                                            headset->GetProjection(),
                                            headset->GetLookAtMatrix());

    Collision3 collision(output);
    collision.normal = hd::normalize(output - headset->GetWorldPosition());

    if (responderState->GetCurrentState() == pointer::SelectionState::FREE) {
        if (mainCamera->ContainsComponent<pointer::MoveResponder>()) {
            auto baseEvent = event->Edit<BaseEvent>();
            auto pointer = event->Edit<pointer::Position>();
            auto move = event->Edit<pointer::Move>();
            auto headset = mainCamera->Edit<Headset>();
            auto moveResponder = mainCamera->Edit<pointer::MoveResponder>();

            if (move->GetControl() == moveResponder->GetControl()) {
                //baseEvent->ConsumeEvent(mainCamera->GetID());

                const float SENSITIVITY = 4.0f;
                const float SENSITIVITY_SCALE_FACTOR = 0.001f;

                currentDelta += pointer->GetDelta();

                float deltaY = currentDelta[0] * SENSITIVITY * SENSITIVITY_SCALE_FACTOR;
                float deltaX = currentDelta[1] * SENSITIVITY * SENSITIVITY_SCALE_FACTOR;
                float deltaZ = currentDelta[2] * SENSITIVITY * SENSITIVITY_SCALE_FACTOR;

                ///
                /// HACK HACK HACK
                ///
                /// What was current[X, Y]Roation used for?  Altering head view?
                ///
                /// The responder takes an entity, but we are simply interested in the collision location.
                ///
                currentYRotation = Quaternion(Axis_Y, deltaY, Rotate_CW);
                currentXRotation = Quaternion(Axis_X, deltaX, Rotate_CW);
                currentZRotation = Quaternion(Axis_Z, deltaZ, Rotate_CW);
                Quaternion result = currentYRotation * currentXRotation * currentZRotation;

                moveResponder->Move(nullptr, collision, pointer->GetDelta());

                //headset->SetOrientationOffset(result);
            }
        }
    }
}


void Cameras::PointerSelectQuery(Entity *event) {
    auto pointer = event->Get<pointer::Position>();
    auto rayTracer = mainCamera->Get<RayTracer>();

    auto responderState = mainCamera->Edit<pointer::ResponderState>();
    auto headset = mainCamera->Edit<Headset>();

    Vec3 output = rayTracer->PointerToWorld(pointer->GetPosition(),
                                            headset->GetViewport(),
                                            headset->GetProjection(),
                                            headset->GetLookAtMatrix());

    if (responderState->GetCurrentState() == pointer::SelectionState::FREE) {
        responderState->Select();

        if (mainCamera->ContainsComponent<pointer::SelectResponder>()) {
            auto responder = mainCamera->Edit<pointer::SelectResponder>();
            auto baseEvent = event->Edit<BaseEvent>();

            responder->Select(nullptr, Collision3(output));
            baseEvent->ConsumeEvent(mainCamera->GetID());
        }
    }
}


void Cameras::PointerDragQuery(Entity *event) {
    auto pointer = event->Get<pointer::Position>();
    auto rayTracer = mainCamera->Get<RayTracer>();

    auto responderState = mainCamera->Edit<pointer::ResponderState>();
    auto headset = mainCamera->Edit<Headset>();

    Vec3 output = rayTracer->PointerToWorld(pointer->GetPosition(),
                                            headset->GetViewport(),
                                            headset->GetProjection(),
                                            headset->GetLookAtMatrix());

    Collision3 collision(output);
    collision.normal = hd::normalize(output - headset->GetWorldPosition());

    if (responderState->GetCurrentState() == pointer::SelectionState::SELECTED ||
        responderState->GetCurrentState() == pointer::SelectionState::DRAGGED) {
        responderState->Drag();

        if (mainCamera->ContainsComponent<pointer::DragResponder>()) {
            auto baseEvent = event->Edit<BaseEvent>();
            auto pointer = event->Get<pointer::Position>();
            auto drag = event->Get<pointer::Drag>();
            auto responder = mainCamera->Edit<pointer::DragResponder>();
            auto headset = mainCamera->Edit<Headset>();

            if (drag->GetControl() == responder->GetControl()) {
                baseEvent->ConsumeEvent(mainCamera->GetID());

                const float SENSITIVITY = 4.0f;
                const float SENSITIVITY_SCALE_FACTOR = 0.001f;

                float deltaY = pointer->GetDelta()[0] * SENSITIVITY * SENSITIVITY_SCALE_FACTOR;
                float deltaX = pointer->GetDelta()[1] * SENSITIVITY * SENSITIVITY_SCALE_FACTOR;

                currentYRotation = currentYRotation * Quaternion(Axis_Y, deltaY, Rotate_CW);
                currentXRotation = currentXRotation * Quaternion(Axis_X, deltaX, Rotate_CW);

                headset->SetOrientationOffset(currentYRotation * currentXRotation);

                responder->Drag(nullptr, collision, pointer->GetDelta());
            }
        }
    }
}


void Cameras::PointerReleaseQuery(Entity *event) {
    auto pointer = event->Get<pointer::Position>();
    auto rayTracer = mainCamera->Get<RayTracer>();

    auto responderState = mainCamera->Edit<pointer::ResponderState>();
    auto headset = mainCamera->Edit<Headset>();

    Vec3 output = rayTracer->PointerToWorld(pointer->GetPosition(),
                                            headset->GetViewport(),
                                            headset->GetProjection(),
                                            headset->GetLookAtMatrix());

    if (responderState->GetCurrentState() == pointer::SelectionState::DRAGGED ||
        responderState->GetCurrentState() == pointer::SelectionState::SELECTED) {
        responderState->Release();

        if (mainCamera->Get<pointer::ReleaseResponder>()) {
            auto responder = mainCamera->Edit<pointer::ReleaseResponder>();
            auto baseEvent = event->Edit<BaseEvent>();

            responder->Release(nullptr, Collision3(output));
            baseEvent->ConsumeEvent(mainCamera->GetID());
        }
    }
}


void Cameras::KeySelectQuery(hd::Entity *event) {
    auto baseEvent = event->Get<BaseEvent>();
    auto keySelect = event->Get<keyboard::KeySelect>();
    hd::log::i("Client received button down: %c\n", keySelect->GetButtonValue());
}


void Cameras::KeyReleaseQuery(hd::Entity *event) {
    auto baseEvent = event->Get<BaseEvent>();
    auto keyRelease = event->Get<keyboard::KeyRelease>();

    hd::log::i("Client received button up: %c\n", keyRelease->GetButtonValue());
}


void Cameras::KeyboardQuery(hd::Entity *event) {
    auto baseEvent = event->Get<BaseEvent>();
    auto keyboard = event->Get<device::Keyboard>();
    auto headset = mainCamera->Edit<Headset>();

    const float MOVEMENT_SPEED = 5.0f;
    const float ROTATION_SPEED = 0.025f;

    Vec3 position = headset->GetPosition();
    Quaternion rotation = headset->GetWorldOrientation();

    Vec3 result, forward, backward, left, right, up, down;

    if (keyboard->IsKeyDown(hd::Device::Keycode::W)) {
        forward = Vec3(0, 0, 1);
    }

    if (keyboard->IsKeyDown(hd::Device::Keycode::A)) {
        left = Vec3(1, 0, 0);
    }

    if (keyboard->IsKeyDown(hd::Device::Keycode::D)) {
        right = Vec3(-1, 0, 0);
    }

    if (keyboard->IsKeyDown(hd::Device::Keycode::S)) {
        backward = Vec3(0, 0, -1);
    }

    result = forward + backward + left + right + up + down;
    hd::normalize(result);
   // result = rotation * result;
    headset->SetPosition(position + (MOVEMENT_SPEED * result));

    
    float rotationX = 0.0f, rotationY = 0.0f;
    if (keyboard->IsKeyDown(hd::Device::Keycode::CURSOR_LEFT)) {
        rotationY = ROTATION_SPEED;
    }

    if (keyboard->IsKeyDown(hd::Device::Keycode::CURSOR_RIGHT)) {
        rotationY = -ROTATION_SPEED;
    }

    if (keyboard->IsKeyDown(hd::Device::Keycode::CURSOR_UP)) {
        rotationX = ROTATION_SPEED;
    }

    if (keyboard->IsKeyDown(hd::Device::Keycode::CURSOR_DOWN)) {
        rotationX = -ROTATION_SPEED;
    }

    Quaternion rot = Quaternion(Vec3(1, 0, 0), rotationX) * Quaternion(Vec3(0, 1, 0), rotationY);

    headset->RotateBy(rot);
}




void Cameras::GamepadAsTouchController(hd::Entity *event) {

}


void Cameras::GamepadQuery(hd::Entity *event) {
    auto gamepad = event->Get<device::Gamepad>();
    auto transform = mainCamera->Get<Transform>();
    auto headset = mainCamera->Edit<Headset>();

    Vec3 position = headset->GetPosition();
    Quaternion rotation = headset->GetWorldOrientation();

    const float ROTATION_SPEED = 0.025f;
    const float MOVEMENT_SPEED = 10.0f;

    if (gamepad->IsControlActive(gamepad::Control::LEFT_THUMBSTICK)) {
        auto axis = gamepad->GetAxis(gamepad::Control::LEFT_THUMBSTICK);

        Vec3 forward = Vec3(0, 0, -axis->GetDirection()[1]);
        Vec3 sideways = Vec3(-axis->GetDirection()[0], 0, 0);

        Vec3 result = forward + sideways;
        hd::normalize(result);
        result = rotation * result;
        headset->SetPosition(position + (MOVEMENT_SPEED * result));
    }

    if (gamepad->IsControlActive(gamepad::Control::RIGHT_THUMBSTICK)) {
        auto axis = gamepad->GetAxis(gamepad::Control::RIGHT_THUMBSTICK);
        
        Quaternion vert(Vec3(0, 1, 0), -axis->GetDirection()[0] * ROTATION_SPEED);
        //Quaternion horiz(Vec3(1, 0, 0), axis->GetDirection()[1] * ROTATION_SPEED);
        
        Quaternion rotXYZ = vert; // * horiz;
        headset->RotateBy(rotXYZ);
    }
}
