#include "EventQueue.h"

#include "core/device/Device.h"

#include "BaseEvent.h"
#include "EventRenderer.h"
#include "pointer/Position.h"
#include "pointer/Select.h"
#include "pointer/Drag.h"
#include "pointer/Move.h"
#include "pointer/Release.h"
#include "device/Gamepad.h"
#include "device/Keyboard.h"
#include "device/MotionController.h"
#include "device/Mouse.h"
#include "gamepad/ControlSelect.h"
#include "gamepad/ControlRelease.h"
#include "keyboard/KeySelect.h"
#include "keyboard/KeyRelease.h"
#include "headset/HMD.h"
#include "headset/HMDDetector.h"

#include "SDL2/SDL.h"

static const int EVENT_ENTITY_POOL_SIZE = 512;
static const int MAX_ACTIVE_GAMEPADS = 32;
static const int MAX_ACTIVE_MOTION_CONTROLLERS = 32;

static const int MOUSE_MOTION_CONTROL_ID = -999;

static const char * PLAYSTATION_3_CONTROLLER = "PLAYSTATION(R)3 Controller";
static const char * PLAYSTATION_4_CONTROLLER = "PLAYSTATION(R)4 Controller";
static const char * PLAYSTATION_5_CONTROLLER = "PS5 Controller";
static const char * XBOX_360_CONTROLLER = "XInput Controller #1";
static const char * XBOX_ONE_CONTROLLER = "XBOX One";


using namespace hd;
using namespace hd::input;

#pragma mark - Public

EventQueue::EventQueue(hd::application::Timer *timer) :
m_entityPool(EVENT_ENTITY_POOL_SIZE),
m_timer(timer),
m_currentEvents(EVENT_ENTITY_POOL_SIZE),
m_currentHMD(nullptr),
m_activeGamepads(MAX_ACTIVE_GAMEPADS),
m_keyboard(nullptr),
m_mouse(nullptr),
m_gamepads(MAX_ACTIVE_GAMEPADS),
m_motionControllers(MAX_ACTIVE_MOTION_CONTROLLERS) {

    m_baseEventPool = new ComponentPool<BaseEvent>(EVENT_ENTITY_POOL_SIZE);
    m_rendererPool  = new ComponentPool<EventRenderer>(EVENT_ENTITY_POOL_SIZE);

    m_pointerPositionPool   = new ComponentPool<pointer::Position>(EVENT_ENTITY_POOL_SIZE);
    m_pointerMovePool       = new ComponentPool<pointer::Move>(EVENT_ENTITY_POOL_SIZE);
    m_pointerSelectPool     = new ComponentPool<pointer::Select>(EVENT_ENTITY_POOL_SIZE);
    m_pointerDragPool       = new ComponentPool<pointer::Drag>(EVENT_ENTITY_POOL_SIZE);
    m_pointerReleasePool    = new ComponentPool<pointer::Release>(EVENT_ENTITY_POOL_SIZE);

    m_keySelectPool = new ComponentPool<keyboard::KeySelect>(EVENT_ENTITY_POOL_SIZE);
    m_keyReleasePool = new ComponentPool<keyboard::KeyRelease>(EVENT_ENTITY_POOL_SIZE);

    m_controlSelectPool = new ComponentPool<gamepad::ControlSelect>(EVENT_ENTITY_POOL_SIZE);
    m_controlReleasePool = new ComponentPool<gamepad::ControlRelease>(EVENT_ENTITY_POOL_SIZE);

    m_keyboardPool = new ComponentPool<device::Keyboard>(EVENT_ENTITY_POOL_SIZE);
    m_mousePool = new ComponentPool<device::Mouse>(EVENT_ENTITY_POOL_SIZE);
    m_motionControllerPool = new ComponentPool<device::MotionController>(EVENT_ENTITY_POOL_SIZE);
    m_gamepadPool = new ComponentPool<device::Gamepad>(EVENT_ENTITY_POOL_SIZE);

    m_mouseInputPolicy = hd::input::MouseInputPolicy::DEFAULT;
}


EventQueue::~EventQueue() {}

bool IsPlaystationController(SDL_JoystickID index) {
    const char *joystickName = SDL_JoystickNameForIndex(index);
    return strcmp(joystickName, PLAYSTATION_3_CONTROLLER) == 0 ||
           strcmp(joystickName, PLAYSTATION_4_CONTROLLER) == 0 ||
           strcmp(joystickName, PLAYSTATION_5_CONTROLLER) == 0;
}

void EventQueue::DiscoverInputDevices() {
    // Assume a keyboard and a mouse

    m_keyboard = m_entityPool.Create();
    auto keyboardState = m_keyboardPool->Get();
    m_keyboard->AddComponent(keyboardState);

    m_mouse = m_entityPool.Create();
    auto mouseState = m_mousePool->Get();
    m_mouse->AddComponent(mouseState);

    if (m_mouseInputPolicy == hd::input::MouseInputPolicy::MOTION_CONTROLLER) {
        auto motionController = m_entityPool.Create();
        auto motionControllerState = m_motionControllerPool->Get();
        motionController->AddComponent(motionControllerState);

        m_motionControllers.Add(motionController);
    }

    // Detect game pads
    if (SDL_NumJoysticks() > 0) {
        SDL_JoystickEventState(SDL_ENABLE);

        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            SDL_Joystick *joystick = SDL_JoystickOpen(i);
            const char *joystickName = SDL_JoystickName(joystick);
            hd::log::e("Joystick found: %s\n", joystickName);
            if (SDL_JoystickGetAttached(joystick)) {

                SDL_JoystickID joystickId = SDL_JoystickInstanceID(joystick);

                if (1) { //strcmp(joystickName, PLAYSTATION_3_CONTROLLER) == 0) {
                    m_activeGamepads.Add(joystick);

                    auto gamepad = m_entityPool.Create();
                    auto gamepadState = m_gamepadPool->Get();
                    gamepadState->SetDeviceId(joystickId);

                    gamepad->AddComponent(gamepadState);

                    m_gamepads.Add(gamepad);
                }
            }
        }
    }

    // Motion controllers

    // Headsets
    hd::input::HMD *hmd = nullptr;
    if ((hmd = hd::input::HMDDetector::DetectOculus())) {
		if (hmd->InitDevice() == 0) {
			m_currentHMD = hmd;
		} else {
			m_currentHMD = new hd::input::HMD();
		}
    } else {
        m_currentHMD = new hd::input::HMD();
    }
}


///
/// Select events -- can only add the event if the key is not already selected.
///
void EventQueue::OnKeySelect(int sourceDeviceId, int scanCode) {
    if (m_keyboard) {
        auto keySelect = m_keySelectPool->Get();
        keySelect->SetDeviceId(sourceDeviceId);
        keySelect->SetButtonValue(scanCode);

        auto keyboard = m_keyboard->Edit<device::Keyboard>();
        if (keyboard) {
            if (keyboard->IsKeyDown(keySelect->GetKeyCode())) {
                keySelect->Free();
            } else {
                Entity *event = m_entityPool.Create();
                CreateBaseEvent(event);
                event->AddComponent(keySelect);

                hd::Device::Keycode keycode = keySelect->GetKeyCode();
                keyboard->AddKey(keycode);
            }
        }
    }
}


void EventQueue::OnKeyRelease(int sourceDeviceId, int scanCode) {
    Entity *event = m_entityPool.Create();
    CreateBaseEvent(event);

    auto keyRelease = m_keyReleasePool->Get();
    keyRelease->SetDeviceId(sourceDeviceId);
    keyRelease->SetButtonValue(scanCode);
    event->AddComponent(keyRelease);

    if (m_keyboard) {
        auto keyboard = m_keyboard->Edit<device::Keyboard>();
        hd::Device::Keycode keycode = keyRelease->GetKeyCode();
        keyboard->RemoveKey(keycode);
    }
}


void EventQueue::OnGamepadControlSelect(int sourceDeviceId, hd::input::gamepad::Control control) {
    auto gamepad = FindGamepad(sourceDeviceId);

    if (gamepad) {
        auto controlSelect = m_controlSelectPool->Get();
        controlSelect->SetDeviceId(sourceDeviceId);
        controlSelect->SetControl(control);

        auto gamepadState = gamepad->Edit<device::Gamepad>();
        if (gamepadState) {
            if (gamepadState->IsControlActive(control)) {
                controlSelect->Free();
            } else {
                Entity *event = m_entityPool.Create();
                CreateBaseEvent(event);
                event->AddComponent(controlSelect);

                gamepadState->AddControl(control);
            }
        }
    }
}


void EventQueue::OnGamepadControlRelease(int sourceDeviceId, hd::input::gamepad::Control control) {
    Entity *event = m_entityPool.Create();
    CreateBaseEvent(event);

    auto controlRelease = m_controlReleasePool->Get();
    controlRelease->SetDeviceId(sourceDeviceId);
    controlRelease->SetControl(control);
    event->AddComponent(controlRelease);

    auto gamepad = FindGamepad(sourceDeviceId);

    if (gamepad) {
        auto gamepadState = gamepad->Edit<device::Gamepad>();
        gamepadState->RemoveControl(control);
    }
}


#pragma mark - Motion controller events

void EventQueue::OnMotionControllerMoved(int sourceDeviceId, Vec3 &position, Vec3 &delta) {

}


void EventQueue::OnMotionControllerRotated(int sourceDeviceId, Quaternion &delta) {

}


#pragma mark - pointer event handlers

void EventQueue::OnPointerMove(hd::input::pointer::Control control, Vec3 &position, Vec3 &delta) {
    Entity *event = m_entityPool.Create();
    CreateBaseEvent(event);

    pointer::Move *move = m_pointerMovePool->Get();
    move->SetControl(control);
    event->AddComponent(move);

    pointer::Position *positionEvent = m_pointerPositionPool->Get();
    positionEvent->SetPosition(position);
    positionEvent->SetDelta(delta);
    event->AddComponent(positionEvent);

    // Treat mouse input as a motion controller
    if (m_mouseInputPolicy == hd::input::MouseInputPolicy::MOTION_CONTROLLER) {
      //  Quaternion diff();
       // OnMotionControllerRotated(MOUSE_MOTION_CONTROL_ID, position, delta);
    }
}


void EventQueue::OnPointerSelect(hd::input::pointer::Button button, hd::input::pointer::Control control, Vec3 &position) {
    Entity *event = m_entityPool.Create();
    CreateBaseEvent(event);

    pointer::Select *select = m_pointerSelectPool->Get();
    select->SetControl(control);
    select->SetButton(button);
    event->AddComponent(select);

    pointer::Position *positionEvent = m_pointerPositionPool->Get();
    positionEvent->SetPosition(position);
    event->AddComponent(positionEvent);
}


void EventQueue::OnPointerDrag(hd::input::pointer::Button button, hd::input::pointer::Control control, Vec3 &position, Vec3 &delta) {
    Entity *event = m_entityPool.Create();
    CreateBaseEvent(event);

    pointer::Drag *drag = m_pointerDragPool->Get();
    drag->SetControl(control);
    drag->SetButton(button);
    event->AddComponent(drag);

    pointer::Position *positionEvent = m_pointerPositionPool->Get();
    positionEvent->SetPosition(position);
    positionEvent->SetDelta(delta);
    event->AddComponent(positionEvent);
}


void EventQueue::OnPointerRelease(hd::input::pointer::Button button, hd::input::pointer::Control control, Vec3 &position) {
    Entity *event = m_entityPool.Create();
    CreateBaseEvent(event);

    pointer::Release *release = m_pointerReleasePool->Get();
    release->SetControl(control);
    release->SetButton(button);
    event->AddComponent(release);

    pointer::Position *positionEvent = m_pointerPositionPool->Get();
    positionEvent->SetPosition(position);
    event->AddComponent(positionEvent);
}


void EventQueue::Update() {
    m_entityPool.Update();

    UpdateExternalEvents();

    { // keyboard
        auto keyboardEvent = m_entityPool.Create();
        CreateBaseEvent(keyboardEvent);

        auto nextKeyboard = m_keyboardPool->Get();
        keyboardEvent->AddComponent(nextKeyboard);

        nextKeyboard->CopyKeyboard(m_keyboard->Get<hd::input::device::Keyboard>());
        nextKeyboard->Update();
    }

    { // mouse
        auto mouseEvent = m_entityPool.Create();
        CreateBaseEvent(mouseEvent);

        auto nextMouse = m_mousePool->Get();
        mouseEvent->AddComponent(nextMouse);

        nextMouse->CopyMouse(m_mouse->Get<hd::input::device::Mouse>());
        nextMouse->Update();
    }

    { // gamepads
        for (uint32_t i = 0; i < m_gamepads.Size(); ++i) {
            auto gamepad = m_gamepads.At(i);

            auto gamepadEvent = m_entityPool.Create();
            CreateBaseEvent(gamepadEvent);

            auto nextGamepad = m_gamepadPool->Get();
            gamepadEvent->AddComponent(nextGamepad);

            nextGamepad->CopyGamepad(gamepad->Get<hd::input::device::Gamepad>());
            nextGamepad->Update();
        }
    }

    { // motion controllers
        for (uint32_t i = 0; i < m_motionControllers.Size(); ++i) {
            auto motionController = m_motionControllers.At(i);

            auto motionControllerEvent = m_entityPool.Create();
            CreateBaseEvent(motionControllerEvent);

            auto nextMotionController = m_motionControllerPool->Get();
            motionControllerEvent->AddComponent(nextMotionController);

            nextMotionController->CopyMotionController(motionController->Get<hd::input::device::MotionController>());
            nextMotionController->Update();
        }
    }

    hd::Vector<Entity *> oldEvents(EVENT_ENTITY_POOL_SIZE);
    m_currentEvents.Clear();

    EntityQuery *events = m_entityPool.Query<BaseEvent>();

    for (uint32_t i = 0; i < events->Size(); ++i) {
        Entity *event = events->At(i);

        BaseEvent *baseEvent = event->Edit<BaseEvent>();

        if (baseEvent->GetFrame() == m_timer->GetCurrentFrame()) {
            m_currentEvents.Add(event);

        } else if (baseEvent->GetFrame() < m_timer->GetCurrentFrame() - 20) {
            oldEvents.Add(event);
        }
    }

    for (uint32_t i = 0; i < oldEvents.Size(); ++i) {
        Entity *entity = oldEvents.At(i);
        m_entityPool.Free(entity);
    }
}

void EventQueue::UpdateExternalEvents() {
    /*
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			hd::log::i("Key down\n");
			this->OnKeySelect(0, event.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			hd::log::i("Key up\n");
			this->OnKeyRelease(0, event.key.keysym.scancode);
			break;
		default:
			break;
		}
	}*/

    // TODO: DEFAULT_DEVICE_ID must be used!!!
    const unsigned int DEFAULT_DEVICE_ID = 0;
    for (uint32_t i = 0; i < m_gamepads.Size(); ++i) {
        auto gamepadDevice = m_gamepads.At(i);
        auto gamepad = gamepadDevice->Edit<hd::input::device::Gamepad>();

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_MOUSEMOTION:
                    hd::log::i("Mouse motion event\n");
                    break;
                case SDL_JOYBALLMOTION:
                    hd::log::i("Joy ball event\n");
                    break;
                case SDL_JOYAXISMOTION:
                {
                    //hd::log::i("Joystick axis motion %d\n", event.jaxis.axis);
                    float value = float(event.jaxis.value) / float(SHRT_MAX);

                    const char *joystickName = SDL_JoystickNameForIndex(event.jaxis.which);
                    if (IsPlaystationController(event.jaxis.which)) {
                        if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_RIGHT_THUMB_HORIZONTAL) {
                            gamepad->UpdateAxis(hd::input::gamepad::Control::RIGHT_THUMBSTICK,
                                                hd::input::HORIZONTAL_AXIS, value);
                        }

                        if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_RIGHT_THUMB_VERTICAL) {
                            gamepad->UpdateAxis(hd::input::gamepad::Control::RIGHT_THUMBSTICK,
                                                hd::input::VERTICAL_AXIS, value);
                        }

                        if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_LEFT_THUMB_HORIZONTAL) {
                            gamepad->UpdateAxis(hd::input::gamepad::Control::LEFT_THUMBSTICK,
                                                hd::input::HORIZONTAL_AXIS, value);
                        }

                        if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_LEFT_THUMB_VERTICAL) {
                            gamepad->UpdateAxis(hd::input::gamepad::Control::LEFT_THUMBSTICK,
                                                hd::input::VERTICAL_AXIS, value);
                        }
                    } else {
                        if (event.jaxis.axis == hd::input::sdl::XBOX_RIGHT_THUMB_HORIZONTAL) {
                            gamepad->UpdateAxis(hd::input::gamepad::Control::RIGHT_THUMBSTICK,
                                                hd::input::HORIZONTAL_AXIS, value);
                        }

                        if (event.jaxis.axis == hd::input::sdl::XBOX_RIGHT_THUMB_VERTICAL) {
                            gamepad->UpdateAxis(hd::input::gamepad::Control::RIGHT_THUMBSTICK,
                                                hd::input::VERTICAL_AXIS, value);
                        }

                        if (event.jaxis.axis == hd::input::sdl::XBOX_LEFT_THUMB_HORIZONTAL) {
                            gamepad->UpdateAxis(hd::input::gamepad::Control::LEFT_THUMBSTICK,
                                                hd::input::HORIZONTAL_AXIS, value);
                        }

                        if (event.jaxis.axis == hd::input::sdl::XBOX_LEFT_THUMB_VERTICAL) {
                            gamepad->UpdateAxis(hd::input::gamepad::Control::LEFT_THUMBSTICK,
                                                hd::input::VERTICAL_AXIS, value);
                        }

                        if (event.jaxis.axis == hd::input::sdl::XBOX_LEFT_TRIGGER) {
                            value = value * 0.5f + 0.5f;
                            gamepad->UpdateAxis(hd::input::gamepad::Control::LEFT_TRIGGER,
                                                hd::input::VERTICAL_AXIS, value);
                            hd::log::i("Left trigger: %3.3f\n", value);
                        }

                        if (event.jaxis.axis == hd::input::sdl::XBOX_RIGHT_TRIGGER) {
                            value = value * 0.5f + 0.5f;
                            gamepad->UpdateAxis(hd::input::gamepad::Control::RIGHT_TRIGGER,
                                                hd::input::VERTICAL_AXIS, value);
                            hd::log::i("Right trigger: %3.3f\n", value);
                        }
                    }

                    //hd::log::i("Joystick axis motion: (%d), %d, %d\n", event.jaxis.axis, event.jaxis.value, event.jaxis.which);
                }
                    break;
                case SDL_JOYBUTTONDOWN:
                {
                    const char *joystickName = SDL_JoystickNameForIndex(event.jaxis.which);
                    hd::log::e("Joystick found: %s\n", joystickName);
                    if (IsPlaystationController(event.jaxis.which)) {
                        if (event.jbutton.state == SDL_PRESSED) {
                            hd::log::i("Button Down %d\n", event.jaxis.axis);
                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_START) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::START_BUTTON);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_CROSS) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::PLAYSTATION_CROSS);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_SQUARE) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::PLAYSTATION_SQUARE);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_TRIANGLE) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::PLAYSTATION_TRIANGLE);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_CIRCLE) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::PLAYSTATION_CIRCLE);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_LEFT_TRIGGER) {
                                gamepad->UpdateAxis(hd::input::gamepad::Control::LEFT_TRIGGER,
                                                    hd::input::VERTICAL_AXIS, 1.0f);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_RIGHT_TRIGGER) {
                                gamepad->UpdateAxis(hd::input::gamepad::Control::RIGHT_TRIGGER,
                                                    hd::input::VERTICAL_AXIS, 1.0f);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_LEFT_BUMPER) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::LEFT_BUMPER);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_RIGHT_BUMPER) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::RIGHT_BUMPER);
                            }

                            hd::log::i("Selected: (%d)\n", event.jbutton.button);
                        }
                    } else {
                        if (event.jbutton.state == SDL_PRESSED) {
                            hd::log::i("Button Down %d\n", event.jaxis.axis);
                            if (event.jaxis.axis == hd::input::sdl::XBOX_START) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::START_BUTTON);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_A) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::XBOX_A);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_X) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::XBOX_X);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_Y) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::XBOX_Y);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_B) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::XBOX_B);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_LEFT_BUMPER) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::LEFT_BUMPER);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_RIGHT_BUMPER) {
                                OnGamepadControlSelect(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::RIGHT_BUMPER);
                            }

                            hd::log::i("Selected: (%d)\n", event.jbutton.button);
                        }
                    }
                }
                    break;
                case SDL_JOYBUTTONUP:
                {
                    const char *joystickName = SDL_JoystickNameForIndex(event.jaxis.which);
                    hd::log::e("Joystick found: %s\n", joystickName);
                    if (IsPlaystationController(event.jaxis.which)) {
                        if (event.jbutton.state == SDL_RELEASED) {
                            hd::log::i("Button Up %d\n", event.jaxis.axis);
                            if (event.jaxis.axis == hd::input::sdl::XBOX_START) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::START_BUTTON);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_CROSS) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::PLAYSTATION_CROSS);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_SQUARE) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::PLAYSTATION_SQUARE);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_TRIANGLE) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::PLAYSTATION_TRIANGLE);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_CIRCLE) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::PLAYSTATION_CIRCLE);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_LEFT_TRIGGER) {
                                gamepad->UpdateAxis(hd::input::gamepad::Control::LEFT_TRIGGER,
                                                    hd::input::VERTICAL_AXIS, 0.1f);
                            }
                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_RIGHT_TRIGGER) {
                                gamepad->UpdateAxis(hd::input::gamepad::Control::RIGHT_TRIGGER,
                                                    hd::input::VERTICAL_AXIS, 0.1f);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_LEFT_BUMPER) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::LEFT_BUMPER);
                            }

                            if (event.jaxis.axis == hd::input::sdl::PS3_DUALSHOCK_RIGHT_BUMPER) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::RIGHT_BUMPER);
                            }

                        }
                    } else {
                        if (event.jbutton.state == SDL_RELEASED) {
                            hd::log::i("Button Up %d\n", event.jaxis.axis);
                            if (event.jaxis.axis == hd::input::sdl::XBOX_START) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::START_BUTTON);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_A) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::XBOX_A);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_X) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::XBOX_X);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_Y) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::XBOX_Y);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_B) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::XBOX_B);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_LEFT_BUMPER) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::LEFT_BUMPER);
                            }

                            if (event.jaxis.axis == hd::input::sdl::XBOX_RIGHT_BUMPER) {
                                OnGamepadControlRelease(DEFAULT_DEVICE_ID, hd::input::gamepad::Control::RIGHT_BUMPER);
                            }
                        }
                    }

                    //OnGamepadButtonDown(0, hd::input::device::GamepadCode::XBOX_THUMBSTICK_LEFT);
                    hd::log::i("Released: (%d)\n", event.jbutton.button);
                }
                    break;
                default:
                    break;
            }
        }

        gamepad->Update();

        ///
        /// HACK HACK HACK
        ///
        /// Axis select and deselect is dependant on an arbitrary threshold -- that is, a size below
        /// which we consider the axis control to be inactive.  This is because different devices have
        /// different sensitivities, and we can't rely on the values sent by the device to determine
        /// whether the control is active or not.  Small vibrations cause values greater than 0
        /// to arrive, but we want to filter these smaller values out before processing further.
        ///
        /// This threshold is checked on update after all the events have been processed.  Update
        /// determines the state of all of the axis controls -- we can only queue select and release
        /// events when we have this updated state info, so we have to do that here.
        ///
        const unsigned CONTROL_ARRAY_SIZE = 4;
        hd::input::gamepad::Control controls[CONTROL_ARRAY_SIZE];
        controls[0] = hd::input::gamepad::Control::LEFT_THUMBSTICK;
        controls[1] = hd::input::gamepad::Control::RIGHT_THUMBSTICK;
        controls[2] = hd::input::gamepad::Control::LEFT_TRIGGER;
        controls[3] = hd::input::gamepad::Control::RIGHT_TRIGGER;

        for (int i = 0; i < CONTROL_ARRAY_SIZE; ++i) {
            hd::input::gamepad::Control control = controls[i];
            auto axis = gamepad->GetAxis(control);
            if (axis) {
                if (axis->GetState() == hd::input::gamepad::AxisState::Activating) {
                    if (!gamepad->IsControlActive(control)) {
                        Entity *ev = m_entityPool.Create();
                        CreateBaseEvent(ev);

                        auto controlSelect = m_controlSelectPool->Get();
                        controlSelect->SetDeviceId(DEFAULT_DEVICE_ID);
                        controlSelect->SetControl(control);
                        ev->AddComponent(controlSelect);

                        gamepad->AddControl(control);
                    }
                } else if (axis->GetState() == hd::input::gamepad::AxisState::Deactivating) {
                    hdAssert(gamepad->IsControlActive(control));
                    Entity *ev = m_entityPool.Create();
                    CreateBaseEvent(ev);

                    auto controlRelease = m_controlReleasePool->Get();
                    controlRelease->SetDeviceId(DEFAULT_DEVICE_ID);
                    controlRelease->SetControl(control);
                    ev->AddComponent(controlRelease);

                    gamepad->RemoveControl(control);
                }
            }
        }
    }
}


const hd::Vector<Entity *> * EventQueue::GetCurrentEvents() const {
    return &m_currentEvents;
}


void EventQueue::DebugRender() {
    EntityQuery *pointerEvents = m_entityPool.Query<BaseEvent, EventRenderer, pointer::Position>();

    EventRenderer::RenderCamera();

    for (uint32_t i = 0; i < pointerEvents->Size(); ++i) {
        Entity *event = pointerEvents->At(i);

        pointer::Position *pointer = event->Edit<pointer::Position>();

        if (event->ContainsComponent<pointer::Select>()) {
            EventRenderer::RenderPointerSelect(pointer);
        }

        if (event->ContainsComponent<pointer::Drag>()) {
            EventRenderer::RenderPointerDrag(pointer);
        }

        if (event->ContainsComponent<pointer::Release>()) {
            EventRenderer::RenderPointerRelease(pointer);
        }
    }
}


#pragma mark - Private

void EventQueue::PrintDebugInfo(Entity *event) {
    BaseEvent *baseEvent = event->Edit<BaseEvent>();
    hdAssert(baseEvent);

    baseEvent->PrintDebugInfo();

    if (event->ContainsComponent<pointer::Position>()) {
        if (event->ContainsComponent<pointer::Select>()) {
            event->GetComponent<pointer::Select>()->PrintDebugInfo();
        }

        if (event->ContainsComponent<pointer::Drag>()) {
            event->GetComponent<pointer::Drag>()->PrintDebugInfo();
        }

        if (event->ContainsComponent<pointer::Release>()) {
            event->GetComponent<pointer::Release>()->PrintDebugInfo();
        }

        pointer::Position *pointer = event->Edit<pointer::Position>();
        pointer->PrintDebugInfo();
    }
}


void EventQueue::CreateBaseEvent(Entity *entity) {
    BaseEvent *baseEvent = m_baseEventPool->Get();
    baseEvent->SetTimestamp(hd::Device::GetMediaTime());
    baseEvent->SetFrame(m_timer->GetCurrentFrame());
    entity->AddComponent(baseEvent);

    EventRenderer *renderer = m_rendererPool->Get();
    entity->AddComponent(renderer);
}


hd::Entity * EventQueue::FindGamepad(int sourceDeviceId) const {
    for (unsigned i = 0; i < m_gamepads.Size(); ++i) {
        auto gamepad = m_gamepads.At(0);
        auto gamepadState = gamepad->Get<device::Gamepad>();
        if (gamepadState && gamepadState->GetDeviceId() == sourceDeviceId) {
            return gamepad;
        }
    }
    return nullptr;
}


hd::Entity * EventQueue::FindMotionController(int sourceDeviceId) const {
    for (unsigned i = 0; i < m_motionControllers.Size(); ++i) {
        auto motionController = m_motionControllers.At(0);
        auto motionControllerState = motionController->Get<device::MotionController>();
        if (motionControllerState && motionControllerState->GetDeviceId() == sourceDeviceId) {
            return motionController;
        }
    }
    return nullptr;
}
