//
// On update, looks for new devices.
//

#ifndef _VAST_GIBS_HMD_DETECTOR_H_
#define _VAST_GIBS_HMD_DETECTOR_H_

#include <string>
#include <Vast/VastMath.h>
#include "gibs/headset/HMD.h"

namespace hd {
    namespace input {
        class HMDDetector {
        public:
            
            static hd::input::HMD * DetectOculus();
            
        };
    }
}

#endif
