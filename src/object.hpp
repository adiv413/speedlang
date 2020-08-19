#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "token.hpp" 
#include <memory>

// OBJECT CLASS STORES *ONLY* LITERALS, NOT IDENTIFIERS
// for identifiers: map identifier names to objects
// for lists: vector<object *>

class object { 
    public:
        std::unique_ptr<object> type_ptr;
        TokenType type;

        object();
        object(Token *t);
        object(int i) : type_ptr(new int_t(i)), type(TokenType::INT) {}
        object(double d) : type_ptr(new double_t(d)), type(TokenType::DOUBLE) {}
        object(bool b) : type_ptr(new bool_t(b)), type(b ? TokenType::TRUE : TokenType::FALSE) {}
        object(std::string s) : type_ptr(new string_t(s)), type(TokenType::STRING) {}
        virtual void* getValue();
};

class int_t : public object { 
    public:
        int value;
        
        int_t(int i) : value(i) {}
        void* getValue() override;
};

class bool_t : public object { 
    public:
        bool value;
        
        bool_t(bool b) : value(b) {} 
        void* getValue() override;
};

class double_t : public object { 
    public:
        double value;
        
        double_t(double d) : value(d) {}
        void* getValue() override;
};

class string_t : public object { 
    public:
        std::string value;
        
        string_t(std::string s) : value(s) {}        
        void* getValue() override;
};

// this class really only exists to significantly shorten the evaluation code (otherwise we'd have to check to see 
// if an expression's left or right is an identifier every time we want to call an evaluate method) and may not be 
// the best in terms of class design
// TODO: try to find a better way to execute this (if possible)

class reference_t : public object { 
    public:
        std::string identifier_name;
        
        reference_t(std::string s) : identifier_name(s) {}        
        void* getValue() override;
};

#endif