#include "Counter.h"

Counter::Counter(unsigned int ID) : Component(ID),
    m_value(0) {
}


uint32_t Counter::GetValue() const {
    return m_value;
}


void Counter::Increment() {
    m_value++;
}


void Counter::Decrement() {
    m_value--;
}