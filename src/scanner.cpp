#include "scanner.hpp"

Scanner::Scanner(std::string raw_contents) : contents(&*raw_contents.begin()), cursor(0), line(0), length(raw_contents.length()) {}

std::vector<Token> Scanner::scan_file_contents() {

    while(cursor < length) {
        char current = contents[cursor];

        switch(current) {
            // whitespace

            case ' ':
            case '\t':
            case '\r':
            case '\v':
            case '\f':
                cursor++;
                break;
            case '\n':
                line++;
                cursor++;
                break;
            
            // operators and brackets

            case '+':
                if(check_next_character('+', TokenType::PLUS_PLUS));
                else if(check_next_character('=', TokenType::PLUS_EQUAL));
                else add_single_char_token(TokenType::PLUS);
                break;
            case '-':
                if(check_next_character('-', TokenType::MINUS_MINUS));
                else if(check_next_character('=', TokenType::MINUS_EQUAL));
                else add_single_char_token(TokenType::MINUS);
                break;
            case '*':
                if(check_next_character('*', TokenType::STAR_STAR));
                else if(check_next_character('=', TokenType::STAR_EQUAL));
                else add_single_char_token(TokenType::STAR);
                break;
            case '/':
                if(check_next_character('=', TokenType::SLASH_EQUAL));
                else if(cursor + 1 < length && contents[cursor + 1] == '/') while(contents[cursor] != '\n') cursor++; // comment case: "//"
                else add_single_char_token(TokenType::SLASH);
                break;
            case '%':
                if(check_next_character('=', TokenType::PERCENT_EQUAL));
                else add_single_char_token(TokenType::PERCENT);
                break;
            case '!':
                if(check_next_character('=', TokenType::NOT_EQUAL));
                else add_single_char_token(TokenType::NOT);
                break;
            case '=':
                if(check_next_character('=', TokenType::EQUAL_EQUAL));
                else add_single_char_token(TokenType::EQUAL);
                break;
            case '^':
                if(check_next_character('=', TokenType::CARET_EQUAL));
                else add_single_char_token(TokenType::CARET);
                break;
            case '>':
                if(check_next_character('=', TokenType::GREATER_EQUAL));
                else add_single_char_token(TokenType::GREATER);
                break;
            case '<':
                if(check_next_character('=', TokenType::LESS_EQUAL));
                else add_single_char_token(TokenType::LESS);
                break;
            case '(':
                add_single_char_token(TokenType::LEFT_PAREN);
                break;
            case ')':
                add_single_char_token(TokenType::RIGHT_PAREN);
                break;
            case '{':
                add_single_char_token(TokenType::LEFT_BRACE);
                break;
            case '}':
                add_single_char_token(TokenType::RIGHT_BRACE);
                break;
            case '[':
                add_single_char_token(TokenType::LEFT_SQ_BRACKET);
                break;
            case ']':
                add_single_char_token(TokenType::RIGHT_SQ_BRACKET);
                break;
            case '?':
                add_single_char_token(TokenType::QUESTION);
                break;
            case ':':
                add_single_char_token(TokenType::COLON);
                break;
            case ';':
                add_single_char_token(TokenType::SEMICOLON);
                break;
            case '.':
                add_single_char_token(TokenType::DOT);
                break;
            case ',':
                add_single_char_token(TokenType::COMMA);
                break;
            
            // literals and identifiers

            case '"':
                add_string();
                break;

            default:
                
        }
    }
}

bool Scanner::check_next_character(char next_char, TokenType type) {
    if(cursor + 1 < length && contents[cursor + 1] == next_char) {
        tokens.push_back(Token(type, line, cursor, std::string(contents + cursor * sizeof(char), 2)));
        cursor += 2;
        return true;
    }

    return false;
}

void Scanner::add_single_char_token(TokenType type) {
    tokens.push_back(Token(type, line, cursor, std::string(1, contents[cursor])));
    cursor++;
}

void Scanner::add_string() {
    int start = ++cursor; // first character in the string

    while(cursor < length && contents[cursor] != '"') {
        cursor++;
    }

    // cursor should now be at the closing quotation mark

    if(cursor == length) {
        throw UNTERMINATED_STRING_EXCEPTION();
    }

    std::string value = std::string(contents + start * sizeof(char), cursor - start);
    tokens.push_back(Token(TokenType::STRING, line, cursor, value));
    cursor++;
}

void Scanner::add_number() {

}

void Scanner::add_identifier() {

}