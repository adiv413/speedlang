#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "object.hpp"
#include "expression.hpp"
#include "error.hpp"
#include "exceptions.hpp"
#include <vector>
#include <cmath>
#include <unordered_map>

class Evaluator {
    private:

    //do type checking in the functions themselves, bc they pass objects anyway

        std::vector<ExprPtr> *expressions;
        std::string filename;
        std::string contents;
        std::unique_ptr<Error> currentError;
        const std::unordered_map<TokenType, std::string> token_type_to_string_map = {
            {TokenType::STRING, "string"},
            {TokenType::INT, "int"},
            {TokenType::DOUBLE, "double"},
            {TokenType::IDENTIFIER, "identifier"},
            {TokenType::NULL_T, "null"}
        };

        object evaluateExpression(ExprPtr *expr);
        object evaluateUnary(Token *op, object operand); //TODO: when doing identifiers, take the token value (identifier name) and match that to an object via symbol table;
        object evaluateBinary(Token *op, object leftOperand, object rightOperand); //TODO: have switch case based on operator type and call helper functions for each operator type
        
        //unary operators

        object op_plus_plus(object *operand);
        object op_minus_minus(object *operand);
        object op_unary_plus(object *operand);
        object op_unary_minus(object *operand);
        object op_not(object *operand);

        //binary operators

        object op_binary_plus(object *leftOperand, object *rightOperand);
        object op_binary_minus(object *leftOperand, object *rightOperand);
        object op_star(object *leftOperand, object *rightOperand);
        object op_slash(object *leftOperand, object *rightOperand);
        object op_percent(object *leftOperand, object *rightOperand);
        object op_caret(object *leftOperand, object *rightOperand);
        object op_greater(object *leftOperand, object *rightOperand);
        object op_less(object *leftOperand, object *rightOperand);
        object op_greater_equal(object *leftOperand, object *rightOperand);
        object op_less_equal(object *leftOperand, object *rightOperand);

        void addError(Token *t, std::string e_type, std::string e_desc);

    public:
        Evaluator(std::vector<ExprPtr> *parser_owned_exprs, std::string file, std::string raw_contents) 
        : expressions(parser_owned_exprs), filename(file), contents(raw_contents) {}
        std::vector<object> evaluate();
};

#endif