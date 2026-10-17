#include "SelectResponder.h"

using namespace hd::input;

pointer::SelectResponder::SelectResponder(unsigned int ID) : Component(ID),
    m_onSelect(),
    m_control(hd::input::pointer::Control::MOUSE),
    m_button(hd::input::pointer::Button::UNKNOWN_BUTTON)
{}


void pointer::SelectResponder::SetButton(hd::input::pointer::Button button) {
    m_button = button;
}


void pointer::SelectResponder::SetControl(hd::input::pointer::Control control) {
    m_control = control;
}


hd::input::pointer::Control pointer::SelectResponder::GetControl() const {
    return m_control;
}


hd::input::pointer::Button pointer::SelectResponder::GetButton() const {
    return m_button;
}


void pointer::SelectResponder::Select(hd::Entity *entity, const Collision3 &collision) {
    if (m_onSelect) {
        m_onSelect(entity, collision);
    }
}