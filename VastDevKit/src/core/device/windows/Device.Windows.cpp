#include "core/Device/Device.h"
#include <Vast/VastMath.h>
#include <chrono>
// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <gl/glew.h>

// Include GLFW
#include <glfw/glfw3.h>

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
	g_currentDeviceWidth = w;
	g_currentDeviceHeight = h;
    // No need to do anything on iOS
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
	auto t = std::chrono::steady_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::duration<double>>(t).count();
}


hd::Device::DeviceIdiom hd::Device::GetDeviceIdiom() {
    return hd::Device::DeviceIdiom::PC;
}


hd::Device::Keycode hd::Device::GetKeyboardCode(const int code) {
    switch (code) {
		/*
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
            break;*/
		case GLFW_KEY_W: //SDL_SCANCODE_W:
            return Keycode::W;
            break;
        case GLFW_KEY_A:
            return Keycode::A;
            break;
        case GLFW_KEY_S:
            return Keycode::S;
            break;
        case GLFW_KEY_D:
            return Keycode::D;
            break;
        /*case 'z':
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
            break;*/
        case '~':
            return Keycode::TILDE;
            break;
        case GLFW_KEY_SPACE:
            return Keycode::SPACE;
            break;
		case GLFW_KEY_TAB://wchar_t(0x00000009):
            return Keycode::TAB;
            break;
        case GLFW_KEY_UP:
            return Keycode::CURSOR_UP;
            break;
        case GLFW_KEY_LEFT:
            return Keycode::CURSOR_LEFT;
            break;
        case GLFW_KEY_RIGHT:
            return Keycode::CURSOR_RIGHT;
            break;
        case GLFW_KEY_DOWN:
            return Keycode::CURSOR_DOWN;
            break;
        case GLFW_KEY_ENTER:
            return Keycode::RETURN;
            break;
        default:
            break;
    }
    hd::log::i("Unknown keycode: %d\n", code);
    return Keycode::UNKNOWN_KEYCODE;
    
}
