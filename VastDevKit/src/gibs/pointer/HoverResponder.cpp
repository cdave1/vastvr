#include "HoverResponder.h"

using namespace hd::input;

pointer::HoverResponder::HoverResponder(unsigned int ID) : Component(ID),
    m_onHover(),
    m_pointerControl(hd::input::pointer::Control::MOUSE) {}


void pointer::HoverResponder::SetPointerControl(hd::input::pointer::Control pointerControl) {
    m_pointerControl = pointerControl;
}


hd::input::pointer::Control pointer::HoverResponder::GetPointerControl() const {
    return m_pointerControl;
}


void pointer::HoverResponder::Hover(hd::Entity *entity, const Vec3 &position) {
    if (m_onHover) {
        m_onHover(entity, position);
    }
}
