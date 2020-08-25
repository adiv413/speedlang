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

    try {
        UnaryFunc func = token_type_to_unary_func_map[op_type];
        return (this->*func)(&operand);
    }
    catch(const std::exception &e) {
        if(token_type_to_string_map.find(operand.type) != token_type_to_string_map.end()) {
            addError(op, "RuntimeError", "operator '" + op->value + "' does not support operand type '" + 
            token_type_to_string_map.at(operand.type) + "'");
        }
        throw UNSUPPORTED_OPERAND_TYPES_ERROR();
    }
}

object Evaluator::evaluateBinary(Token *op, object leftOperand, object rightOperand) {
    TokenType op_type = op->token_type;

    try {
        BinaryFunc func = token_type_to_binary_func_map[op_type];
        return (this->*func)(&leftOperand, &rightOperand);
    }
    catch(const std::exception &e) {
        if(token_type_to_string_map.find(leftOperand.type) != token_type_to_string_map.end() && 
            token_type_to_string_map.find(rightOperand.type) != token_type_to_string_map.end()) {

            addError(op, "RuntimeError", "operator '" + op->value + "' does not support operand types '" + 
            token_type_to_string_map.at(leftOperand.type) + "', '" + 
            token_type_to_string_map.at(rightOperand.type) + "'");
        }
        throw UNSUPPORTED_OPERAND_TYPES_ERROR();
    }
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
    TokenType left = leftOperand->type;
    TokenType right = rightOperand->type;

    switch(left) {
        case TokenType::STRING:
            {
                std::string leftValue = *static_cast<std::string *>(leftOperand->getValue()); 

                switch(right) {
                    case TokenType::STRING:
                        {
                            std::string rightValue = *static_cast<std::string *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::INT:
                        {
                            int rightValue = *static_cast<int *>(rightOperand->getValue());
                            return object(leftValue + std::to_string(rightValue));
                        }
                    case TokenType::DOUBLE:
                        {
                            double rightValue = *static_cast<double *>(rightOperand->getValue());
                            return object(leftValue + std::to_string(rightValue));
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                    case TokenType::NULL_T:
                        {
                            return object(leftValue + token_type_to_string_map.at(TokenType::NULL_T));
                        }
                }
                break;
            }
        case TokenType::INT:
           { 
               int leftValue = *static_cast<int *>(leftOperand->getValue());

               switch(right) {
                    case TokenType::STRING: 
                        {
                            std::string rightValue = *static_cast<std::string *>(rightOperand->getValue());
                            return object(std::to_string(leftValue) + rightValue);
                        }
                    case TokenType::INT: 
                        {
                            int rightValue = *static_cast<int *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            double rightValue = *static_cast<double *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                    case TokenType::NULL_T: 
                        {
                            throw UNSUPPORTED_OPERAND_TYPES_ERROR();
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                double leftValue = *static_cast<double *>(rightOperand->getValue());

                switch(right) {
                    case TokenType::STRING: 
                        {
                            std::string rightValue = *static_cast<std::string *>(rightOperand->getValue());
                            return object(std::to_string(leftValue) + rightValue);
                        }
                    case TokenType::INT: 
                        {
                            int rightValue = *static_cast<int *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            double rightValue = *static_cast<double *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                    case TokenType::NULL_T: 
                        {
                            throw UNSUPPORTED_OPERAND_TYPES_ERROR();
                        }
                }
                break;
            }

        case TokenType::IDENTIFIER: //TODO: implement
            {
                switch(right) {
                    case TokenType::STRING: 
                        {
                            std::string rightValue = *static_cast<std::string *>(rightOperand->getValue());
                        }
                    case TokenType::INT: 
                        {
                            int rightValue = *static_cast<int *>(rightOperand->getValue());
                        }
                    case TokenType::DOUBLE: 
                        {
                            double rightValue = *static_cast<double *>(rightOperand->getValue());
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                    case TokenType::NULL_T: 
                        {
                            //TODO: implement (if its a string then its good)
                        }
                }
                break;
            }

        case TokenType::NULL_T:
            {
                switch(right) {
                    case TokenType::STRING: 
                        {
                            std::string rightValue = *static_cast<std::string *>(rightOperand->getValue());
                            return object(token_type_to_string_map.at(TokenType::NULL_T) + rightValue);
                        }
                    case TokenType::INT: 
                        {
                            throw UNSUPPORTED_OPERAND_TYPES_ERROR();
                        }
                    case TokenType::DOUBLE: 
                        {
                            throw UNSUPPORTED_OPERAND_TYPES_ERROR();
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                    case TokenType::NULL_T: 
                        {
                            return object(nullptr);
                        }
                }
                break;
            }
    }
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