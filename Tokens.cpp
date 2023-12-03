#include "Tokens.h"

Tokens::Tokens(TokenType type, const std::string& value) {
    this->type = type;
    this->value = value;
}

TokenType Tokens::getType() const {
    return this->type;
}

const std::string& Tokens::getValue() const {
    return this->value;
}
