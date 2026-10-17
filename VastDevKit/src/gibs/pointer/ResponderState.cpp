#include "ResponderState.h"

using namespace hd::input;

pointer::ResponderState::ResponderState(unsigned int ID) : Component(ID),
    m_selectionState(SelectionState::FREE), m_lookAtState(LookAtState::FREE) {}


void pointer::ResponderState::Select() {
    m_selectionState = SelectionState::SELECTED;
}


void pointer::ResponderState::Drag() {
    m_selectionState = SelectionState::DRAGGED;
}


void pointer::ResponderState::Release() {
    m_selectionState = SelectionState::FREE;
}


void pointer::ResponderState::LookAt() {
    m_lookAtState = LookAtState::LOOKAT;
}


void pointer::ResponderState::UnLookAt() {
    m_lookAtState = LookAtState::FREE;
}
