#include "HMDDetector.h"
#include <Vast/VastGraphics.h>
#include "gibs/headset/oculus/OculusHMD.h"
//#include "OVR_CAPI.h"

hd::input::HMD * hd::input::HMDDetector::DetectOculus() {
#ifdef OVR_CAPI_h
    hd::input::OculusHMD *hmd = new hd::input::OculusHMD();
    return hmd;
#else
    return nullptr;
#endif
}
