#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "token.hpp"
#include "object.hpp"

class Expression {
    public:
        Expression *left;
        Expression *right;
        Token *op;

        Expression(Expression *l, Expression *r, Token *t) : left(l), right(r), op(t) {}
        ~Expression();

        // check token types with operators, if they are good then create objects and evaluate
        // have a base lookup table of operator actions and types and evaluate based on that (allows for operator overloading)
        object evaluate(); 
        
};

#endif