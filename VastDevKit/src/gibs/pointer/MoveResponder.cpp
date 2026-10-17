#include "MoveResponder.h"

using namespace hd::input;

pointer::MoveResponder::MoveResponder(unsigned int ID) : Component(ID),
    m_onMove(),
    m_control(hd::input::pointer::Control::MOUSE) {}


void pointer::MoveResponder::SetControl(hd::input::pointer::Control pointerControl) {
    m_control = pointerControl;
}


hd::input::pointer::Control pointer::MoveResponder::GetControl() const {
    return m_control;
}


void pointer::MoveResponder::Move(hd::Entity *entity, const Collision3 &collision, const Vec3 &delta) {
    if (m_onMove) {
        m_onMove(entity, collision, delta);
    }
}
