#include "scanner.hpp"

Scanner::Scanner(std::string file, std::string raw_contents) 
: contents(raw_contents), cursor(0), line(0), length(raw_contents.length()), line_begin(0), filename(file), errorOccurred(false) {}

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
                line_begin = cursor;
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
                if(check_next_character('=', TokenType::STAR_EQUAL));
                else add_single_char_token(TokenType::STAR);
                break;
            case '/':
                if(check_next_character('=', TokenType::SLASH_EQUAL));
                else if(cursor + 1 < length && contents[cursor + 1] == '/') while(contents[cursor + 1] != '\n') cursor++; // comment case: "//"
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
                if(cursor + 1 < length && '0' <= contents[cursor + 1] && contents[cursor + 1] <= '9') add_number();
                else add_single_char_token(TokenType::DOT);
                break;
            case ',':
                add_single_char_token(TokenType::COMMA);
                break;
            
            // literals and identifiers

            case '\'':
            case '"':
                add_string();
                break;

            default:
                if('0' <= current && current <= '9') add_number();
                else add_identifier();
        }
    }

    for(auto error : errors) {
        error.print();
    }

    return tokens;
}

bool Scanner::check_next_character(char next_char, TokenType type) {
    if(cursor + 1 < length && contents[cursor + 1] == next_char) {
        tokens.push_back(Token(type, line, line_begin, cursor - line_begin, std::string(contents, cursor, 2)));
        cursor += 2;
        return true;
    }

    return false;
}

void Scanner::add_single_char_token(TokenType type) {
    tokens.push_back(Token(type, line, line_begin, cursor - line_begin, std::string(1, contents[cursor])));
    cursor++;
}

void Scanner::add_string() {
    // strings can be made using either double or single quotes, but the starting quote has to match the ending quote
    // for example, 'Hello, World!' is valid and "Hello, World!" is valid, but 'Hello, World!" and "Hello, World!' are not

    char quot_mark = contents[cursor]; 
    int start = ++cursor; // first character in the string (not quotation mark)

    while(cursor < length && contents[cursor] != quot_mark) {
        cursor++;
    }

    // cursor should now be at the closing quotation mark

    if(cursor == length) {
        add_error("SyntaxError", "Unterminated string", start - 1 - line_begin);
    }
    else {
        std::string value = std::string(contents, start, cursor - start);
        tokens.push_back(Token(TokenType::STRING, line, line_begin, cursor - line_begin, value));
        cursor++;
    }
}

// used for parsing both doubles and ints

void Scanner::add_number() {
    bool isDouble = false;
    int start = cursor;
    char curr = contents[cursor];

    while(cursor < length && ('0' <= curr && curr <= '9' || curr == '.')) {
        if(curr == '.') isDouble = true;
        curr = contents[++cursor];
    }

    if(isDouble) {
        std::string val(contents, start, cursor - start);
        if(val[0] == '.') val = "0" + val; 
        else if(val[val.length() - 1] == '.') val += "0";

        tokens.push_back(Token(TokenType::DOUBLE, line, line_begin, start - line_begin, val));
    }
    else {
        std::string val(contents, start, cursor - start);
        tokens.push_back(Token(TokenType::INT, line, line_begin, start - line_begin, val));
    }
}

void Scanner::add_identifier() {
    cursor++;
}

void Scanner::add_error(std::string e_type, std::string e_desc, int col) {
    int line_end = line_begin;
    while(line_end < length && contents[line_end] != '\n') line_end++;
    std::string e_line(contents, line_begin, line_end - line_begin);

    errors.push_back(Error(e_type, e_line, e_desc, filename, line, col));
    errorOccurred = true;
}