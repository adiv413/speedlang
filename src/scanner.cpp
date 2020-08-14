#include "scanner.hpp"

Scanner::Scanner(std::string file, std::string raw_contents) 
: contents(raw_contents), cursor(0), line(1), length(raw_contents.length()), line_begin(0), filename(file), errorOccurred(false) {}

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
                if(tokens.size() != 0 && tokens.back().token_type != TokenType::BACKSLASH) {
                    add_single_char_token(TokenType::NEWLINE);
                    line++;
                    line_begin = cursor;
                    break;
                }
                else if(tokens.size() != 0 && tokens.back().token_type == TokenType::BACKSLASH) tokens.pop_back();

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
                else if(cursor + 1 < length && contents[cursor + 1] == '/') while(cursor < length && contents[cursor] != '\n') cursor++; // comment case: "//"
                else if(cursor + 1 < length && contents[cursor + 1] == '*') parse_block_comment();
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
            case '\\':
                add_single_char_token(TokenType::BACKSLASH);
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
                else if(check_identifier(current)) add_identifier();
                else {
                    add_error("SyntaxError", "Unexpected character", cursor - line_begin); 
                    cursor++;
                }
                break;
        }
    }

    for(auto error : errors) {
        error.print();
    }

    removeDuplicateNewlines();

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
    int start = cursor;

    while(check_identifier(contents[cursor])) cursor++;
    std::string identifier_name(contents, start, cursor - start);

    if(reserved_keywords.find(identifier_name) != reserved_keywords.end()) {
        tokens.push_back(Token(reserved_keywords.at(identifier_name), line, line_begin, start - line_begin, identifier_name));
    }
    else {
        tokens.push_back(Token(TokenType::IDENTIFIER, line, line_begin, start - line_begin, identifier_name));
    }
}

void Scanner::add_error(std::string e_type, std::string e_desc, int col) {
    int line_end = line_begin;
    while(line_end < length && contents[line_end] != '\n') line_end++;
    std::string e_line(contents, line_begin, line_end - line_begin);

    errors.push_back(Error(e_type, e_line, e_desc, filename, line, col));
    errorOccurred = true;
}

bool Scanner::check_identifier(char c) {
    return isalpha(c) || c == '_' || c == '$' || c == '-';
}

void Scanner::parse_block_comment() {
    int blk_com_begin = cursor - line_begin;
    int num_newlines = 0;
    int temp_line_begin = line_begin;

    while(cursor + 1 < length && contents.substr(cursor, 2) != "*/") {
        if(contents[cursor] == '\n') {
            num_newlines++;
            temp_line_begin = cursor + 1;
        }
        cursor++;
    }

    if(cursor + 1 >= length && contents.substr(contents.length() - 2, 2) != "*/") {
        add_error("SyntaxError", "Unclosed block comment at end of file", blk_com_begin);
    }

    line += num_newlines;
    line_begin = temp_line_begin;
    cursor += 2;
}

void Scanner::removeDuplicateNewlines() {
    TokenType target = TokenType::NEWLINE;
    
    int curr = 0;
    while(curr < tokens.size()) {
        if(curr + 1 < tokens.size() && tokens[curr].token_type == target && tokens[curr + 1].token_type == target) {
            int end = ++curr;
            while(end < tokens.size() && tokens[end].token_type == target) end++;
            tokens.erase(tokens.begin() + curr, tokens.begin() + end);
        }
        curr++;
    }
}