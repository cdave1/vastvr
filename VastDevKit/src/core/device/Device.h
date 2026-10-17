#ifndef _VAST_DEVICE_H_
#define _VAST_DEVICE_H_

namespace hd {
    class Device {
    public:
        enum class DeviceIdiom {
            Undefined = 0,
            Phone,
            Tablet,
            PC,
        };
        
        
        enum class DeviceOrientation {
            Landscape = 0,
            Portrait,
        };
        
        
        enum class Keycode {
            KEY_1,
            KEY_2,
            KEY_3,
            W,
            A,
            S,
            D,
            F,
            G,
            Z,
            X,
            C,
            V,
            SPACE,
            TILDE,
            TAB,
            CURSOR_UP,
            CURSOR_DOWN,
            CURSOR_LEFT,
            CURSOR_RIGHT,
            RETURN,
            UNKNOWN_KEYCODE
        };
        
        
#ifdef ANDROID
        void SetJavaClassRef(void * proxy);
        
        void SetUniqueDeviceId(const char *udid);
#endif
        
        void SetDeviceOrientation(hd::Device::DeviceOrientation orientation);
        
        static hd::Device::DeviceOrientation GetDeviceOrientation();
        
        static float GetPixelScale();
        
        static void SetDisplayScreenSize(const double w, const double h);
        
        static float GetDisplayScreenWidth();
        
        static float GetDisplayScreenHeight();
        
        static float GetAspectRatio();
        
        static const char * GetUniqueDeviceId();
        
        static hdTimestamp GetMediaTime();
        
        static hd::Device::DeviceIdiom GetDeviceIdiom();
        
        static hd::Device::Keycode GetKeyboardCode(const wchar_t code);
        
    };
}

#endif
