#ifndef TokenType_H
#define TokenType_H

#include <string>

enum TokenType 
{
    NUM_DEC, 
    NUM_INT, 
    ID,
    Int,
    Float,
    Bool,
    String, 
    COMMENT, 
    RESERVED_KEYWORD, 
    BINARY_OPERATOR, 
    OPEN_PAREN, 
    CLOSE_PAREN, 
    Equals, 
    Semicolon, 
    Codeblock, 
    ENDCODEBLOCK, 
    Symbol, 
    COMPARATOR_SYMBOLS, 
    Char,
    PlusPlus,
    MinusMinus,
    EOFF,
    _return,
    OpenCurlyBrace,
    CloseCurlyBrace,
    Functio,
    Comma,
    If,
    While,
    Else,
    equalsequals,
    Noteequals,
    Plusequals,
    Minusequals,
    Starequals,
    Slashequals,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Boolean,
    Case,
    Switch,
    Default,
    Colon,
    For,
    LessThan
};

struct Token {
    std::string value;
    TokenType type;
};

#endif 
