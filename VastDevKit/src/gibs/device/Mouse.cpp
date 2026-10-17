#include "Mouse.h"
#include "SDL2/SDL.h"

using namespace hd::input;

device::Mouse::Mouse(unsigned int ID) : Component(ID),
  m_deviceId(hd::input::UnspecifiedDeviceId),
  m_currentScreenPosition()
{}


int device::Mouse::GetDeviceId() const {
    return m_deviceId;
}


void device::Mouse::Reset() {
}


void device::Mouse::Update() {
}


void device::Mouse::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


void device::Mouse::CopyMouse(const Mouse *mouse) {
    m_currentScreenPosition = mouse->GetCurrentScreenPosition();
}
