#include "DragResponder.h"

using namespace hd::input;

pointer::DragResponder::DragResponder(unsigned int ID) : Component(ID),
    m_onDrag(),
    m_control(hd::input::pointer::Control::MOUSE),
    m_button(hd::input::pointer::Button::UNKNOWN_BUTTON)
{}


void pointer::DragResponder::SetButton(hd::input::pointer::Button button) {
    m_button = button;
}


void pointer::DragResponder::SetControl(hd::input::pointer::Control control) {
    m_control = control;
}


hd::input::pointer::Control pointer::DragResponder::GetControl() const {
    return m_control;
}


hd::input::pointer::Button pointer::DragResponder::GetButton() const {
    return m_button;
}


void pointer::DragResponder::Drag(hd::Entity *entity, const Collision3 &collision, const Vec3 &delta) {
    if (m_onDrag) {
        m_onDrag(entity, collision, delta);
    }
}
