#include "KeySelect.h"

hd::input::keyboard::KeySelect::KeySelect(unsigned int ID) : Component(ID),
    m_deviceId(hd::input::UnspecifiedDeviceId),
    m_buttonValue(hd::input::NullButtonValue),
    m_keyCode(hd::Device::Keycode::UNKNOWN_KEYCODE)
{}


void hd::input::keyboard::KeySelect::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


int hd::input::keyboard::KeySelect::GetDeviceId() const {
    return m_deviceId;
}


wchar_t hd::input::keyboard::KeySelect::GetButtonValue() const {
    return m_buttonValue;
}


hd::Device::Keycode hd::input::keyboard::KeySelect::GetKeyCode() const {
    return m_keyCode;
}


void hd::input::keyboard::KeySelect::PrintDebugInfo() const {
    hd::log::i("Button Down, ID: %d\n", m_deviceId);
}
