#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "token.hpp"
#include "exceptions.hpp"
#include "error.hpp"
#include <vector>
#include <string>
#include <cctype>

class Scanner {
    private:
        int cursor;
        int line;
        int line_begin;
        int length;
        std::string contents;
        std::string filename;
        std::vector<Token> tokens;
        std::vector<Error> errors;
        bool errorOccurred;

        bool check_next_character(char next_char, TokenType type);
        void add_single_char_token(TokenType type);
        void add_string();
        void add_number();
        void add_identifier();
        void add_error(std::string e_type, std::string e_desc, int col);

    public:
        Scanner(std::string file, std::string raw_contents);
        std::vector<Token> scan_file_contents();
};

#endif