#include "BooleanOption.h"

using namespace hd::ui;

BooleanOption::BooleanOption(unsigned int ID) : Component(ID),
    m_currentState(false) {
}


void BooleanOption::On(hd::Entity *sender) {
    m_currentState = true;
    if (m_onOptionChanged) {
        m_onOptionChanged(m_currentState, sender);
    }
}


void BooleanOption::Off(hd::Entity *sender) {
    m_currentState = false;
    if (m_onOptionChanged) {
        m_onOptionChanged(m_currentState, sender);
    }
}


void BooleanOption::OnOptionChanged(const std::function<void(bool state, hd::Entity *sender)> &callback) {
    m_onOptionChanged = callback;
}