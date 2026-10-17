#include "Select.h"

using namespace hd::input;

pointer::Select::Select(unsigned int ID) : Component(ID),
    m_control(hd::input::pointer::Control::MOUSE),
    m_button(hd::input::pointer::Button::LEFT_BUTTON)
{}


hd::input::pointer::Control pointer::Select::GetControl() const {
    return m_control;
}


hd::input::pointer::Button pointer::Select::GetButton() const {
    return m_button;
}


void pointer::Select::SetControl(hd::input::pointer::Control control) {
    m_control = control;
}


void pointer::Select::SetButton(hd::input::pointer::Button button) {
    m_button = button;
}


void pointer::Select::PrintDebugInfo() const {
    hd::log::i("\tPointer Select, control: %d button: %d\n", m_control, m_button);
}
