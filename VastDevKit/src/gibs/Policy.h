#ifndef _VAST_INPUT_POLICY_H_
#define _VAST_INPUT_POLICY_H_

namespace hd {
    namespace input {
        //
        // What the event queue uses to determine what to do with mouse events.  It will either treat the
        // event like a mouse move event
        //
        enum class MouseInputPolicy {
            DEFAULT,
            MOTION_CONTROLLER,
        };
    }
}

#endif