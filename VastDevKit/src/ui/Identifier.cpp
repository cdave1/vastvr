#include "Identifier.h"

using namespace hd::ui;

Identifier::Identifier(unsigned int ID) : Component(ID),
    m_identifier() {
}


const std::string & Identifier::GetId() const {
    return m_identifier;
}


void Identifier::SetId(const std::string &identifier) {
    m_identifier = identifier;
}
