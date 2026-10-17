#include "Drag.h"

using namespace hd::input;

pointer::Drag::Drag(unsigned int ID) : Component(ID),
    m_control(hd::input::pointer::Control::MOUSE),
    m_button(hd::input::pointer::Button::LEFT_BUTTON)
{}


hd::input::pointer::Control pointer::Drag::GetControl() const {
    return m_control;
}


hd::input::pointer::Button pointer::Drag::GetButton() const {
    return m_button;
}


void pointer::Drag::SetControl(hd::input::pointer::Control control) {
    m_control = control;
}


void pointer::Drag::SetButton(hd::input::pointer::Button button) {
    m_button = button;
}


void pointer::Drag::PrintDebugInfo() const {
    hd::log::i("\tPointer Drag, control: %d button: %d\n", m_control, m_button);
}
