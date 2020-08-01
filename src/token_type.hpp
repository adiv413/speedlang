#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

enum class TokenType {
    // one character token types

    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    NOT,
    EQUAL,
    CARET,
    GREATER,
    LESS,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_SQ_BRACKET,
    RIGHT_SQ_BRACKET,
    QUESTION,
    COLON,
    SEMICOLON,
    DOT,
    COMMA,

    //two character token types

    EQUAL_EQUAL,
    NOT_EQUAL,
    GREATER_EQUAL,
    LESS_EQUAL,
    PLUS_PLUS,
    MINUS_MINUS,
    PLUS_EQUAL,
    MINUS_EQUAL,
    STAR_EQUAL,
    SLASH_EQUAL,
    PERCENT_EQUAL,
    CARET_EQUAL,

    //multi-character token types (literals and identifiers)

    STRING,
    INT,
    DOUBLE,
    IDENTIFIER,

    //keywords

    AND,
    OR,
    XOR,
    IF,
    ELSE,
    FOR,
    WHILE,
    IN,
    FUNC,
    RETURN,
    TRUE,
    FALSE

};

#endif