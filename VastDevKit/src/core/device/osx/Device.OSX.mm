#include "Device.h"
#include <Vast/VastMath.h>

#import <Appkit/Appkit.h>
#include <Carbon/Carbon.h>
#import "QuartzCore/QuartzCore.h"

static hd::Device::DeviceOrientation g_orientation = hd::Device::DeviceOrientation::Landscape;
static double g_currentDeviceWidth = 0.0f;
static double g_currentDeviceHeight = 0.0f;

void hd::Device::SetDeviceOrientation(hd::Device::DeviceOrientation orientation) {
    g_orientation = orientation;
}


hd::Device::DeviceOrientation hd::Device::GetDeviceOrientation() {
    return g_orientation;
}


float hd::Device::GetPixelScale() {
    float scale = 1.0f;
    return scale;
}


void hd::Device::SetDisplayScreenSize(const double w, const double h) {
    // No need to do anything on iOS
    NSRect bounds = [[NSScreen mainScreen] frame];
    g_currentDeviceWidth = w; //hd::clamp(w, 1.0, double(bounds.size.width));
    g_currentDeviceHeight = h; //hd::clamp(h, 1.0, double(bounds.size.height));
}


float hd::Device::GetDisplayScreenWidth() {
    return g_currentDeviceWidth;
}


float hd::Device::GetDisplayScreenHeight() {
    return g_currentDeviceHeight;
}


float hd::Device::GetAspectRatio() {
    hdAssert(hd::Device::GetDisplayScreenHeight() != 0.0f);
    return hd::Device::GetDisplayScreenWidth() / hd::Device::GetDisplayScreenHeight();
}


const char * hd::Device::GetUniqueDeviceId() {
    return "???";
}


hdTimestamp hd::Device::GetMediaTime() {
    return (hdTimestamp)CACurrentMediaTime();
}


extern hd::Device::DeviceIdiom hd::Device::GetDeviceIdiom() {
    return hd::Device::DeviceIdiom::PC;
}


hd::Device::Keycode hd::Device::GetKeyboardCode(const wchar_t code) {
    switch (code) {
        case '1':
            return Keycode::KEY_1;
            break;
        case '2':
            return Keycode::KEY_2;
            break;
        case '3':
            return Keycode::KEY_3;
            break;
        case '4':
        case '5':
        case '6':
        case '7':
            break;
        case 'w':
            return Keycode::W;
            break;
        case 'a':
            return Keycode::A;
            break;
        case 's':
            return Keycode::S;
            break;
        case 'd':
            return Keycode::D;
            break;
        case 'f':
            return Keycode::F;
            break;
        case 'g':
            return Keycode::G;
            break;
        case 'z':
            return Keycode::Z;
            break;
        case 'x':
            return Keycode::X;
            break;
        case 'c':
            return Keycode::C;
            break;
        case 'v':
            return Keycode::V;
            break;
        case '~':
            return Keycode::TILDE;
            break;
        case ' ':
            return Keycode::SPACE;
            break;
        case wchar_t(0x00000009):
            return Keycode::TAB;
            break;
        case wchar_t(0x0000f700):
            return Keycode::CURSOR_UP;
            break;
        case wchar_t(0x0000f702):
            return Keycode::CURSOR_LEFT;
            break;
        case wchar_t(0x0000f703):
            return Keycode::CURSOR_RIGHT;
            break;
        case wchar_t(0x0000f701):
            return Keycode::CURSOR_DOWN;
            break;
        case wchar_t('\r'):
            return Keycode::RETURN;
            break;
        default:
            break;
    }
    hd::log::i("Unknown keycode: %c\n", code);
    return Keycode::UNKNOWN_KEYCODE;
    
}
