#include "Move.h"

using namespace hd::input;

pointer::Move::Move(unsigned int ID) : Component(ID),
    m_control(hd::input::pointer::Control::MOUSE) {}


void pointer::Move::SetControl(hd::input::pointer::Control control) {
    m_control = control;
}


hd::input::pointer::Control pointer::Move::GetControl() const {
    return m_control;
}


void pointer::Move::PrintDebugInfo() const {
    hd::log::i("\tPointer Move, ID: %d\n", m_control);
}
