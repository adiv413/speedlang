#include "evaluator.hpp"

std::vector<object> Evaluator::evaluate() {
    std::vector<object> ret;

    try {
        for(int i = 0; i < expressions->size(); i++) { 
            ret.push_back(evaluateExpression(&expressions->at(i))); 
        }
    }
    catch(const std::exception &e) {
        if(currentError) {
            currentError->print();
            currentError = nullptr;
        }
        else {
            std::cerr << "RuntimeError: " << e.what();
        }
    }

    return ret;
}

object Evaluator::evaluateExpression(ExprPtr *expr) { 
    // if the expression is an operand, return the operand literal
    if(!expr->get()->left && !expr->get()->right) {
        return object(expr->get()->op.get());
    }
    //postfix unary operator, only right is nullptr
    else if(expr->get()->left && !expr->get()->right) {
        return evaluateUnary(expr->get()->op.get(), evaluateExpression(&expr->get()->left));
    }
    //prefix unary operator, only left is nullptr
    else if(!expr->get()->left && expr->get()->right) {
        return evaluateUnary(expr->get()->op.get(), evaluateExpression(&expr->get()->right));
    }
    else {
        return evaluateBinary(expr->get()->op.get(), evaluateExpression(&expr->get()->left), evaluateExpression(&expr->get()->left));
    }
}

object Evaluator::evaluateUnary(Token *op, object operand) {
    TokenType op_type = op->token_type;

    switch(op_type) {
        case TokenType::PLUS_PLUS:
            try {
                return op_plus_plus(&operand);
            }
            catch(const std::exception &e) {
                if(token_type_to_string_map.find(operand.type) != token_type_to_string_map.end()) {
                    addError(op, "RuntimeError", "operator '" + op->value + "' does not support operand type '" + 
                    token_type_to_string_map.at(operand.type) + "'");
                }
                throw UNSUPPORTED_OPERAND_TYPES_ERROR();
            }
        case TokenType::MINUS_MINUS:
            try {
                return op_minus_minus(&operand);
            }
            catch(const std::exception &e) {
                if(token_type_to_string_map.find(operand.type) != token_type_to_string_map.end()) {
                    addError(op, "RuntimeError", "operator '" + op->value + "' does not support operand type '" + 
                    token_type_to_string_map.at(operand.type) + "'");
                }
                throw UNSUPPORTED_OPERAND_TYPES_ERROR();
            }
        case TokenType::PLUS:
            try {
                return op_unary_plus(&operand);
            }
            catch(const std::exception &e) {
                if(token_type_to_string_map.find(operand.type) != token_type_to_string_map.end()) {
                    addError(op, "RuntimeError", "operator '" + op->value + "' does not support operand type '" + 
                    token_type_to_string_map.at(operand.type) + "'");
                }
                throw UNSUPPORTED_OPERAND_TYPES_ERROR();
            }
        case TokenType::MINUS:
            try {
                return op_unary_minus(&operand);
            }
            catch(const std::exception &e) {
                if(token_type_to_string_map.find(operand.type) != token_type_to_string_map.end()) {
                    addError(op, "RuntimeError", "operator '" + op->value + "' does not support operand type '" + 
                    token_type_to_string_map.at(operand.type) + "'");
                }
                throw UNSUPPORTED_OPERAND_TYPES_ERROR();
            }
        case TokenType::NOT:
            try {
                return op_not(&operand);
            }
            catch(const std::exception &e) {
                if(token_type_to_string_map.find(operand.type) != token_type_to_string_map.end()) {
                    addError(op, "RuntimeError", "operator '" + op->value + "' does not support operand type '" + 
                    token_type_to_string_map.at(operand.type) + "'");
                }
                throw UNSUPPORTED_OPERAND_TYPES_ERROR();
            }
    }
    
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::evaluateBinary(Token *op, object leftOperand, object rightOperand) {

}

object Evaluator::op_plus_plus(object *operand) {
    if(operand->type != TokenType::IDENTIFIER) {
        throw UNSUPPORTED_OPERAND_TYPES_ERROR();
    }

    //TODO: after identifiers are done, implementation
    return object();
}

object Evaluator::op_minus_minus(object *operand) {
    if(operand->type != TokenType::IDENTIFIER) {
        throw UNSUPPORTED_OPERAND_TYPES_ERROR();
    }

    //TODO: after identifiers are done, implementation
    return object();
}

object Evaluator::op_unary_plus(object *operand) {
    TokenType op = operand->type;

    switch(op) {
        case TokenType::INT:
            int ret = *static_cast<int *>(operand->getValue());
            return object(ret >= 0 ? ret : -ret);
        case TokenType::DOUBLE:
            return object(std::fabs(*static_cast<double *>(operand->getValue())));
        case TokenType::IDENTIFIER:
            //TODO: implement
            break;
        default:
            break;
    } 

    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_unary_minus(object *operand) {
    TokenType op = operand->type;

    switch(op) {
        case TokenType::INT:
            return object(-*static_cast<int *>(operand->getValue()));
        case TokenType::DOUBLE:
            return object(-*static_cast<double *>(operand->getValue()));
        case TokenType::IDENTIFIER:
            //TODO: implement
            break;
        default:
            break;
    } 
    
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_not(object *operand) {
    TokenType op = operand->type;

    switch(op) {
        case TokenType::TRUE:
            return object(false);
        case TokenType::FALSE:
            return object(true);
        case TokenType::IDENTIFIER:
            //TODO: implement
            break;
        default:
            break;
    } 

    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_binary_plus(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_binary_minus(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_star(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_slash(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_percent(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_caret(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_greater(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_less(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_greater_equal(object *leftOperand, object *rightOperand) {

}

object Evaluator::op_less_equal(object *leftOperand, object *rightOperand) {

}


void Evaluator::addError(Token *t, std::string e_type, std::string e_desc) {
    int start = t->line_begin;
    int end = start;

    while(end < contents.length() && end != '\n') end++;

    std::string e_line(contents, start, end - start);

    currentError = std::unique_ptr<Error>(new Error(e_type, e_line, e_desc, filename, t->line_number, t->col_number));
}