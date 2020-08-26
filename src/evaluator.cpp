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
        UnaryFunc func = token_type_to_unary_func_map.at(op_type);
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
        BinaryFunc func = token_type_to_binary_func_map.at(op_type);
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
            {
                ll ret = *static_cast<ll *>(operand->getValue());
                return object(ret >= 0 ? ret : -ret);
            }
        case TokenType::DOUBLE:
            return object(std::fabs(*static_cast<ld *>(operand->getValue())));
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
            return object(-*static_cast<ll *>(operand->getValue()));
        case TokenType::DOUBLE:
            return object(-*static_cast<ld *>(operand->getValue()));
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
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue + std::to_string(rightValue));
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
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
               ll leftValue = *static_cast<ll *>(leftOperand->getValue());

               switch(right) {
                    case TokenType::STRING: 
                        {
                            std::string rightValue = *static_cast<std::string *>(rightOperand->getValue());
                            return object(std::to_string(leftValue) + rightValue);
                        }
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(rightOperand->getValue());

                switch(right) {
                    case TokenType::STRING: 
                        {
                            std::string rightValue = *static_cast<std::string *>(rightOperand->getValue());
                            return object(std::to_string(leftValue) + rightValue);
                        }
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue + rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
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
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
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
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_binary_minus(object *leftOperand, object *rightOperand) {
    TokenType left = leftOperand->type;
    TokenType right = rightOperand->type;

    switch(left) {
        case TokenType::INT:
            {
                ll leftValue = *static_cast<ll *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue - rightValue);
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue - rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue - rightValue);
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue - rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::IDENTIFIER:
            {
                //TODO: implementation (get value)

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_star(object *leftOperand, object *rightOperand) { //TODO: add support for string * int, int * string
    TokenType left = leftOperand->type;
    TokenType right = rightOperand->type;

    switch(left) {
        case TokenType::INT:
            {
                ll leftValue = *static_cast<ll *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue * rightValue);
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue * rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue * rightValue);
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue * rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::IDENTIFIER:
            {
                //TODO: implementation (get value)

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_slash(object *leftOperand, object *rightOperand) {
    TokenType left = leftOperand->type;
    TokenType right = rightOperand->type;

    switch(left) {
        case TokenType::INT:
            {
                ll leftValue = *static_cast<ll *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue / rightValue);
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue / rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue / rightValue);
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue / rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::IDENTIFIER:
            {
                //TODO: implementation (get value)

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_percent(object *leftOperand, object *rightOperand) {
    TokenType left = leftOperand->type;
    TokenType right = rightOperand->type;

    switch(left) {
        case TokenType::INT:
            {
                ll leftValue = *static_cast<ll *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue % rightValue);
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(std::fmod(leftValue, rightValue));
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(std::fmod(leftValue, rightValue));
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(std::fmod(leftValue, rightValue));
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::IDENTIFIER:
            {
                //TODO: implementation (get value)

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_caret(object *leftOperand, object *rightOperand) {
    TokenType left = leftOperand->type;
    TokenType right = rightOperand->type;

    switch(left) {
        case TokenType::INT:
            {
                ll leftValue = *static_cast<ll *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: //TODO: should int ^ int be an int or double (rn its a double)
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(static_cast<ld>(std::pow(leftValue, rightValue)));
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(static_cast<ld>(std::pow(leftValue, rightValue)));
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(leftOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(static_cast<ld>(std::pow(leftValue, rightValue)));
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(static_cast<ld>(std::pow(leftValue, rightValue)));
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::IDENTIFIER:
            {
                //TODO: implementation (get value)

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::DOUBLE:
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            //TODO: implement
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_greater(object *leftOperand, object *rightOperand) { 
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
                            return object(leftValue > rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        case TokenType::INT:
           { 
               ll leftValue = *static_cast<ll *>(leftOperand->getValue());

               switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue > rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue > rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(rightOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue > rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue > rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
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
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_less(object *leftOperand, object *rightOperand) {
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
                            return object(leftValue < rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        case TokenType::INT:
           { 
               ll leftValue = *static_cast<ll *>(leftOperand->getValue());

               switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue < rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue < rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(rightOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue < rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue < rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
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
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_greater_equal(object *leftOperand, object *rightOperand) {
    object greater = op_greater(leftOperand, rightOperand);
    if(!*static_cast<bool *>(greater.getValue())) {
        object equal = op_equal_equal(leftOperand, rightOperand);
        if(!*static_cast<bool *>(equal.getValue())) {
            return object(false);
        }
    }

    return object(true);
}

object Evaluator::op_less_equal(object *leftOperand, object *rightOperand) {
    object less = op_less(leftOperand, rightOperand);
    if(!*static_cast<bool *>(less.getValue())) {
        object equal = op_equal_equal(leftOperand, rightOperand);
        if(!*static_cast<bool *>(equal.getValue())) {
            return object(false);
        }
    }
    
    return object(true);
}

object Evaluator::op_equal_equal(object *leftOperand, object *rightOperand) {
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
                            return object(leftValue == rightValue);
                        }
                    case TokenType::IDENTIFIER:
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        case TokenType::INT:
           { 
               ll leftValue = *static_cast<ll *>(leftOperand->getValue());

               switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue == rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue == rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::DOUBLE:
            {
                ld leftValue = *static_cast<ld *>(rightOperand->getValue());

                switch(right) {
                    case TokenType::INT: 
                        {
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                            return object(leftValue == rightValue);
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                            return object(leftValue == rightValue);
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
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
                            ll rightValue = *static_cast<ll *>(rightOperand->getValue());
                        }
                    case TokenType::DOUBLE: 
                        {
                            ld rightValue = *static_cast<ld *>(rightOperand->getValue());
                        }
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                    case TokenType::NULL_T: 
                        {
                            //TODO: implement 
                        }
                }
                break;
            }

        case TokenType::NULL_T:
            {
                switch(right) {
                    case TokenType::NULL_T: 
                        return object(true);
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }
    }
    return object(false);
}

object Evaluator::op_not_equal(object *leftOperand, object *rightOperand) {
    return object(!*static_cast<bool *>(op_equal_equal(leftOperand, rightOperand).getValue()));
}

object Evaluator::op_and(object *leftOperand, object *rightOperand) { 
    TokenType left = leftOperand->type;
    TokenType right = rightOperand->type;

    switch(left) {
        case TokenType::TRUE:
            {
                switch(right) {
                    case TokenType::TRUE:
                        return object(true);
                    case TokenType::NULL_T:
                    case TokenType::FALSE:
                        return object(false);
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }
        case TokenType::NULL_T:
        case TokenType::FALSE:
                return object(false);
        case TokenType::IDENTIFIER: //TODO: implement
            {
                switch(right) {
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

object Evaluator::op_or(object *leftOperand, object *rightOperand) { 
    TokenType left = leftOperand->type;
    TokenType right = rightOperand->type;

    switch(left) {
        case TokenType::TRUE:
            return object(true);
        case TokenType::NULL_T:
        case TokenType::FALSE:
            {
                switch(right) {
                    case TokenType::TRUE:
                        return object(true);
                    case TokenType::NULL_T:
                    case TokenType::FALSE:
                        return object(false);
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }

        case TokenType::IDENTIFIER: //TODO: implement
            {
                switch(right) {
                    case TokenType::IDENTIFIER: 
                        {
                            //TODO: implement
                        }
                }
                break;
            }
    }
    throw UNSUPPORTED_OPERAND_TYPES_ERROR();
}

void Evaluator::addError(Token *t, std::string e_type, std::string e_desc) {
    int start = t->line_begin;
    int end = start;

    while(end < contents.length() && end != '\n') end++;

    std::string e_line(contents, start, end - start);

    currentError = std::unique_ptr<Error>(new Error(e_type, e_line, e_desc, filename, t->line_number, t->col_number));
}