#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "token.hpp" 
#include <memory>

// OBJECT CLASS STORES *ONLY* LITERALS, NOT IDENTIFIERS
// for identifiers: map identifier names to objects
// for lists: vector<object *>

using ll = long long;
using ld = long double;

class object { 
    public:
        std::unique_ptr<object> type_ptr;
        TokenType type;

        object();
        object(Token *t);
        object(ll i);
        object(ld d);
        object(bool b);
        object(std::string s);
        object(std::nullptr_t n);
        virtual void* getValue();
};

class int_t : public object { 
    public:
        ll value;
        
        int_t(ll i) : value(i) {}
        void* getValue() override;
};

class bool_t : public object { 
    public:
        bool value;
        
        bool_t(bool b) : value(b) {} 
        void* getValue() override;
};

class ldouble_t : public object { 
    public:
        ld value;
        
        ldouble_t(ld d) : value(d) {}
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

typedef std::shared_ptr<object> ObjectPtr;

#endif