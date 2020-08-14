#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "token.hpp"
#include "object.hpp"
#include <memory>
#include <utility>

class Expression {
    public:
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
        std::unique_ptr<Token> op;

        Expression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, std::unique_ptr<Token> t) : left(std::move(l)), right(std::move(r)), op(std::move(t)) {}

        // check token types with operators, if they are good then create objects and evaluate
        // have a base lookup table of operator actions and types and evaluate based on that (allows for operator overloading)
        object evaluate(); 
        
};

typedef std::unique_ptr<Expression> ExprPtr;
typedef std::unique_ptr<Token> TokenPtr;

#endif