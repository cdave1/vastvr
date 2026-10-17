#include "KeyRelease.h"

hd::input::keyboard::KeyRelease::KeyRelease(unsigned int ID) : Component(ID),
    m_deviceId(hd::input::UnspecifiedDeviceId),
    m_buttonValue(hd::input::NullButtonValue),
    m_keyCode(hd::Device::Keycode::UNKNOWN_KEYCODE)
{}


void hd::input::keyboard::KeyRelease::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


int hd::input::keyboard::KeyRelease::GetDeviceId() const {
    return m_deviceId;
}


wchar_t hd::input::keyboard::KeyRelease::GetButtonValue() const {
    return m_buttonValue;
}


hd::Device::Keycode hd::input::keyboard::KeyRelease::GetKeyCode() const {
    return m_keyCode;
}


void hd::input::keyboard::KeyRelease::PrintDebugInfo() const {
    hd::log::i("Button Up, ID: %d\n", m_deviceId);
}