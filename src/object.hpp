#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include "token.hpp" 

// for identifiers: map identifier names to objects
// for lists: vector<object *>

class object { // rvalues, really just a wrapper for a void *
    public:
        void *value;
        TokenType type;
        object(Token t);
};

#endif