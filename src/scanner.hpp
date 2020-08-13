#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "token.hpp"
#include "exceptions.hpp"
#include "error.hpp"
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>

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
        std::unordered_map<std::string, TokenType> reserved_keywords = {
            {"and", TokenType::AND},
            {"or", TokenType::OR},
            {"not", TokenType::NOT},
            {"xor", TokenType::XOR},
            {"if", TokenType::IF},
            {"else", TokenType::ELSE},
            {"for", TokenType::FOR},
            {"while", TokenType::WHILE},
            {"in", TokenType::IN},
            {"func", TokenType::FUNC},
            {"return", TokenType::RETURN},
            {"true", TokenType::TRUE},
            {"false", TokenType::FALSE},
            {"class", TokenType::CLASS},
            {"public", TokenType::PUBLIC},
            {"private", TokenType::PRIVATE},
            {"static", TokenType::STATIC},
            {"null", TokenType::NULL_T}
        };

        bool check_next_character(char next_char, TokenType type);
        bool check_identifier(char c);
        void add_single_char_token(TokenType type);
        void add_string();
        void add_number();
        void add_identifier();
        void add_error(std::string e_type, std::string e_desc, int col);
        void parse_block_comment();
        void removeDuplicateNewlines();

    public:
        Scanner(std::string file, std::string raw_contents);
        std::vector<Token> scan_file_contents();
        bool errorOccurred;
};

#endif