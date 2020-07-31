#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "token.hpp"
#include "errors.hpp"
#include <vector>
#include <string>

class Scanner {
    private:
        int cursor;
        int line;
        int length;
        char *contents;
        std::vector<Token> tokens;

        bool check_next_character(char next_char, TokenType type);
        void add_single_char_token(TokenType type);
        void add_string();
        void add_number();
        void add_identifier();

    public:
        Scanner(std::string raw_contents);
        std::vector<Token> scan_file_contents();
};

#endif