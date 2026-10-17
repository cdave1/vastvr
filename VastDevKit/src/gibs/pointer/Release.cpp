#include "Release.h"

using namespace hd::input;

pointer::Release::Release(unsigned int ID) : Component(ID),
    m_control(hd::input::pointer::Control::MOUSE),
    m_button(hd::input::pointer::Button::LEFT_BUTTON)
{}


hd::input::pointer::Control pointer::Release::GetControl() const {
    return m_control;
}


hd::input::pointer::Button pointer::Release::GetButton() const {
    return m_button;
}


void pointer::Release::SetControl(hd::input::pointer::Control control) {
    m_control = control;
}


void pointer::Release::SetButton(hd::input::pointer::Button button) {
    m_button = button;
}


void pointer::Release::PrintDebugInfo() const {
    hd::log::i("\tPointer Release, control: %d button: %d\n", m_control, m_button);
}
