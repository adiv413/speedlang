#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "token.hpp" 
#include <memory>

// for identifiers: map identifier names to objects
// for lists: vector<object *>

class object { 
    public:
        std::unique_ptr<object> type_ptr;
        TokenType type;

        object();
        object(Token t);
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

#endif