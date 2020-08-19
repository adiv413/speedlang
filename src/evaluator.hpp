#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "object.hpp"
#include "expression.hpp"
#include <vector>
#include <unordered_map>
#include <cstdlib>

class Evaluator {
    private:

    //do type checking in the functions themselves, bc they pass objects anyway
        

        std::vector<ExprPtr> *expressions;
        object evaluateExpression(ExprPtr *expr);
        object evaluateUnary(TokenType op, object operand); //TODO: when doing identifiers, take the token value (identifier name) and match that to an object via symbol table;
        object evaluateBinary(TokenType op, object leftOperand, object rightOperand); //TODO: have switch case based on operator type and call helper functions for each operator type
        object op_plus_plus(object operand);
        object op_minus_minus(object operand);
        object op_unary_plus(object operand);
        object op_unary_minus(object operand);
        object op_not(object operand);

    public:
        Evaluator(std::vector<ExprPtr> *parser_owned_exprs) : expressions(parser_owned_exprs) {}
        std::vector<object> evaluate();
};

#endif