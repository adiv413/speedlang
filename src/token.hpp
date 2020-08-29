#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "token_type.hpp"

class Token {
    public:
        TokenType token_type;
        int line_number;
        int line_begin; // number of characters from the beginning of file contents to the beginning of the line that this token falls on
        int col_number; 
        std::string value;

        Token(TokenType type, int line, int begin, int col, std::string val) 
        : token_type(type), line_number(line), line_begin(begin), col_number(col), value(val) {}



        Token(const Token &t) 
        : token_type(t.token_type), line_number(t.line_number), line_begin(t.line_begin), col_number(t.col_number), value(t.value) {}
};

#endif