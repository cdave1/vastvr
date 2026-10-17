///
/// Translate raw input from the device into game specific input.  No translation should occur here -- there should be
/// analogies for every single possible device that we care about.
///
#ifndef _VAST_INPUT_EVENT_QUEUE_H_
#define _VAST_INPUT_EVENT_QUEUE_H_

/**
 * Goals:
 *  - Anything can store information here, from any thread, at any
 *    time.
 *  - Stores batched input.
 *  - When the application processing thread processes items on the queue
 *    the current frame number will be incremented.
 *  - Lockless!
 *  - Input events are tied to a frame number to ensure input is not 
 *    blocked while input is received.
 *  - Incrementing the frame number should be atomic.
 *
 * * Weird stuff:
 *   mouse has pointerId,
 *   what about button? left vs right vs scroll wheel)
 *      - left, right, scroll all different pointers
 *      - mouse move (wihout engaging left, right, scroll) is a special type
 */

#include <Vast/VastMath.h>
#include "core/application/Timer.h"
#include "core/entity/EntityPool.h"
#include "core/entity/Entity.h"
#include "core/component/ComponentPool.h"
#include "gibs/Policy.h"
#include "gibs/pointer/PointerCode.h"
#include "gibs/gamepad/GamepadCode.h"

namespace hd {
    namespace application {
        class Timer;
    }
    
    namespace input {
        class BaseEvent;
        class DeviceDetector;
        class EventRenderer;
        class HMD;
        
        namespace pointer {
            class Position;
            class Move;
            class Select;
            class Drag;
            class Release;
        }

        namespace device {
            class Gamepad;
            class Keyboard;
            class MotionController;
            class Mouse;
        }

        namespace gamepad {
            class ControlSelect;
            class ControlRelease;
        }

        namespace keyboard {
            class KeySelect;
            class KeyHeld;
            class KeyRelease;
        }
        
        
        class EventQueue {
        private:
            
            EntityPool m_entityPool;
        
            const hd::application::Timer *m_timer;
            
            hd::Vector<Entity *> m_currentEvents;
            
            hd::input::HMD *m_currentHMD;
            
            ComponentPool<BaseEvent>            *m_baseEventPool;
            ComponentPool<EventRenderer>        *m_rendererPool;
            ComponentPool<pointer::Position>    *m_pointerPositionPool;
            ComponentPool<pointer::Select>      *m_pointerSelectPool;
            ComponentPool<pointer::Move>        *m_pointerMovePool;
            ComponentPool<pointer::Drag>        *m_pointerDragPool;
            ComponentPool<pointer::Release>     *m_pointerReleasePool;
            ComponentPool<keyboard::KeySelect>  *m_keySelectPool;
            ComponentPool<keyboard::KeyRelease> *m_keyReleasePool;

            ComponentPool<gamepad::ControlSelect> *m_controlSelectPool;
            ComponentPool<gamepad::ControlRelease>   *m_controlReleasePool;

            ComponentPool<device::Keyboard> *m_keyboardPool;
            ComponentPool<device::Mouse> *m_mousePool;
            ComponentPool<device::MotionController>  *m_motionControllerPool;
            ComponentPool<device::Gamepad>  *m_gamepadPool;

            hd::Vector<SDL_Joystick *> m_activeGamepads;

            hd::Entity *m_keyboard;
            hd::Entity *m_mouse;

            hd::Vector<hd::Entity *> m_gamepads;
            hd::Vector<hd::Entity *> m_motionControllers;

            hd::input::MouseInputPolicy m_mouseInputPolicy;

            
        public:
            
            EventQueue(hd::application::Timer *timer);
            
            ~EventQueue();
            
            void DiscoverInputDevices();
            
            void OnKeySelect(int sourceDeviceId, int scanCode);
            
            void OnKeyRelease(int sourceDeviceId, int scanCode);
            
            void OnGamepadControlSelect(int sourceDeviceId, hd::input::gamepad::Control control);
            
            void OnGamepadControlRelease(int sourceDeviceId, hd::input::gamepad::Control control);

            void OnAxisControlChanged(int sourceDeviceId,
                                      hd::input::gamepad::Control control,
                                      hd::input::gamepad::AxisName axis,
                                      const float normalizedValue);

            void OnMotionControllerMoved(int sourceDeviceId, Vec3 &position, Vec3 &delta);

            void OnMotionControllerRotated(int sourceDeviceId, Quaternion &delta);
            
            void OnPointerSelect(hd::input::pointer::Button button, hd::input::pointer::Control control, Vec3 &position);
            
            void OnPointerDrag(hd::input::pointer::Button button, hd::input::pointer::Control control, Vec3 &position, Vec3 &delta);
            
            void OnPointerRelease(hd::input::pointer::Button button, hd::input::pointer::Control control, Vec3 &position);

            void OnPointerMove(hd::input::pointer::Control control, Vec3 &position, Vec3 &delta);
            
            void Update();
            
            void DebugRender();
            
            const hd::Vector<Entity *> * GetCurrentEvents() const;
            
            HMD * GetCurrentHMD() const { return m_currentHMD; }
            
            
        private:
            
            void PrintDebugInfo(Entity *event);
            
            void UpdateExternalEvents();
            
            void CreateBaseEvent(Entity *entity);

            hd::Entity * FindGamepad(int sourceDeviceId) const;

            hd::Entity * FindMotionController(int sourceDeviceId) const;

        };
    }
}


#endif
