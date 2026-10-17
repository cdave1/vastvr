#include "Gamepad.h"
#include "SDL2/SDL.h"

static const float FMAX_AXIS_THRESHOLD = 0.15;

hd::input::device::Gamepad::Gamepad(unsigned int ID) : Component(ID),
  m_deviceId(hd::input::UnspecifiedDeviceId),
  m_controls(),
  m_axisControls()
{}


int hd::input::device::Gamepad::GetDeviceId() const {
    return m_deviceId;
}


void hd::input::device::Gamepad::Reset() {
    m_controls.clear();
    m_axisControls.clear();
}


void hd::input::device::Gamepad::Update() {
    for (auto i = m_axisControls.begin(); i != m_axisControls.end(); ++i) {
        if (i->second.GetDirection().Length() < FMAX_AXIS_THRESHOLD) {
            if (i->second.GetState() == hd::input::gamepad::AxisState::Active ||
                i->second.GetState() == hd::input::gamepad::AxisState::Activating) {
                i->second.SetState(hd::input::gamepad::AxisState::Deactivating);
            } else if (i->second.GetState() == hd::input::gamepad::AxisState::Deactivating) {
                i->second.SetState(hd::input::gamepad::AxisState::Inactive);
                m_controls.erase(i->second.GetControl());
            }
        } else {
            if (i->second.GetState() == hd::input::gamepad::AxisState::Activating) {
                i->second.SetState(hd::input::gamepad::AxisState::Active);
            } else if (i->second.GetState() == hd::input::gamepad::AxisState::Deactivating ||
                       i->second.GetState() == hd::input::gamepad::AxisState::Inactive) {
                i->second.SetState(hd::input::gamepad::AxisState::Activating);
            }
        }
    }
}


void hd::input::device::Gamepad::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


void hd::input::device::Gamepad::UpdateAxis(hd::input::gamepad::Control control,
                                            hd::input::gamepad::AxisName axis,
                                            const float value) {
    if (m_axisControls.count(control) == 0) {
        hd::input::gamepad::Axis a(control, Vec2());
        m_axisControls[control] = a;
    }
    
    Vec2 direction = m_axisControls[control].GetDirection();
    if (axis == hd::input::HORIZONTAL_AXIS) {
        direction[0] = value;
    } else if (axis == hd::input::VERTICAL_AXIS) {
        direction[1] = value;
    }
    m_axisControls[control].SetDirection(direction);
}


void hd::input::device::Gamepad::SetAxis(hd::input::gamepad::Control control, const Vec2 &direction) {
    if (m_axisControls.count(control) == 0) {
        hd::input::gamepad::Axis a(control, Vec2());
        m_axisControls[control] = a;
    }
    m_axisControls[control].SetDirection(direction);
}


bool hd::input::device::Gamepad::IsControlActive(hd::input::gamepad::Control control) const {
    return m_controls.count(control) > 0;
}


const hd::input::gamepad::Axis * hd::input::device::Gamepad::GetAxis(hd::input::gamepad::Control control) const {
    if (m_axisControls.count(control) == 0) {
        return NULL;
    }
    
    const hd::input::gamepad::Axis *axis = &(m_axisControls.at(control));
    return axis;
}


void hd::input::device::Gamepad::AddControl(hd::input::gamepad::Control control) {
    m_controls[control] = 1;
}


void hd::input::device::Gamepad::RemoveControl(hd::input::gamepad::Control control) {
    m_controls.erase(control);
}


void hd::input::device::Gamepad::CopyGamepad(const Gamepad *gamepad) {
    { // buttons
        std::map<hd::input::gamepad::Control, int> buttons = gamepad->GetControls();
        for (auto iter = buttons.begin(); iter != buttons.end(); ++iter) {
            this->AddControl(iter->first);
        }
    }
    
    { // axis controls
        std::map<hd::input::gamepad::Control, hd::input::gamepad::Axis> axisControls = gamepad->GetAxisControls();
        for (auto iter = axisControls.begin(); iter != axisControls.end(); ++iter) {
            this->SetAxis(iter->first, iter->second.GetDirection());
        }
    }
}
