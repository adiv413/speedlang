#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "token_type.hpp"

class Token {
    public:
        TokenType token_type;
        int line_number;
        int character_number;
        std::string value;

        Token(TokenType type, int line, int character, std::string val) : token_type(type), line_number(line), character_number(character), value(val) {}
};

#endif