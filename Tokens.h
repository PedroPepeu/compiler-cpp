#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include "TokenType.h"

class Tokens {
private:
    TokenType type;
    std::string value;

public:
    Tokens(TokenType type, const std::string& value);
    TokenType getType() const;
    const std::string& getValue() const;
};

#endif
