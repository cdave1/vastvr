#ifndef _VAST_GIBS_DEVICE_POINTERCODE_H_
#define _VAST_GIBS_DEVICE_POINTERCODE_H_

namespace hd {
    namespace input {
        namespace pointer {
            enum class Button {
                UNKNOWN_BUTTON,
                LEFT_BUTTON,
                RIGHT_BUTTON,
                PRIMARY_SCROLL_WHEEL_BUTTON
            };

            // Mouse have
            // buttons
            // axis controls
            // 

            enum class Control {
                UNKNOWN_CONTROL,
                MOUSE,
                PRIMARY_SCROLL_WHEEL,
                SECONDARY_SCROLL_WHEEL
            };
        }
    }
};

#endif