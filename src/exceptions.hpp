#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class FILE_NOT_FOUND_EXCEPTION : public std::exception {
    private:
        std::string message;
    public:
        FILE_NOT_FOUND_EXCEPTION(std::string filename) 
        : message("File \"" + filename + "\" not found") {}

        virtual const char* what() const throw()
        {
            return this->message.c_str();
        }
};

class UNCLOSED_PARENTHETICAL_SYNTAX_ERROR : public std::exception {
    private:
        std::string message;
    public:
        UNCLOSED_PARENTHETICAL_SYNTAX_ERROR() 
        : message("Expected ')' to close parenthetical expression") {}

        virtual const char* what() const throw()
        {
            return this->message.c_str();
        }
};

class EXPECTED_EXPRESSION_SYNTAX_ERROR : public std::exception {
    private:
        std::string message;
    public:
        EXPECTED_EXPRESSION_SYNTAX_ERROR() 
        : message("Expected expression; current token cannot start an expression") {}

        virtual const char* what() const throw()
        {
            return this->message.c_str();
        }
};

class MISSING_LINE_SEPARATOR_SYNTAX_ERROR : public std::exception {
    private:
        std::string message;
    public:
        MISSING_LINE_SEPARATOR_SYNTAX_ERROR() 
        : message("Expected semicolon or newline after expression") {}

        virtual const char* what() const throw()
        {
            return this->message.c_str();
        }
};

class UNSUPPORTED_OPERAND_TYPES_ERROR : public std::exception {
    private:
        std::string message;
    public:
        UNSUPPORTED_OPERAND_TYPES_ERROR() 
        : message("Unsupported operand type(s) for operator") {}

        virtual const char* what() const throw()
        {
            return this->message.c_str();
        }
};

#endif