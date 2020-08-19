#include "evaluator.hpp"

std::vector<object> Evaluator::evaluate() {
    std::vector<object> ret;

    for(int i = 0; i < expressions->size(); i++) { 
        ret.push_back(evaluateExpression(&expressions->at(i))); //TODO: wrap in try catch
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
        return evaluateUnary(expr->get()->op->token_type, evaluateExpression(&expr->get()->left));
    }
    //prefix unary operator, only left is nullptr
    else if(!expr->get()->left && expr->get()->right) {
        return evaluateUnary(expr->get()->op->token_type, evaluateExpression(&expr->get()->right));
    }
    else {
        return evaluateBinary(expr->get()->op->token_type, evaluateExpression(&expr->get()->left), evaluateExpression(&expr->get()->left));
    }
}

object Evaluator::evaluateUnary(TokenType op, object operand) {
    switch(op) {
        case TokenType::PLUS_PLUS:
            //TODO: implement
            break;
        case TokenType::MINUS_MINUS:
            //TODO: implement
            break;
        case TokenType::PLUS:
            //TODO: implement
            break;
        case TokenType::MINUS:
            //TODO: implement
            break;
        case TokenType::NOT:
            //TODO: implement
            break;
    }
}

object Evaluator::op_plus_plus(object operand) {
    if(operand.type != TokenType::IDENTIFIER) {
        //TODO: throw error
    }

    //TODO: after identifiers are done, implementation
    return object();
}

object Evaluator::op_minus_minus(object operand) {
    if(operand.type != TokenType::IDENTIFIER) {
        //TODO: throw error
    }

    //TODO: after identifiers are done, implementation
    return object();
}

object Evaluator::op_unary_plus(object operand) {
    TokenType op = operand.type;

    switch(op) {
        case TokenType::INT:
            int ret = *static_cast<int *>(operand.getValue());
            return object(ret >= 0 ? ret : -ret);
        case TokenType::DOUBLE:
            return object(std::abs(*static_cast<double *>(operand.getValue())));
        case TokenType::IDENTIFIER:
            //TODO: implement
            break;
        default:
            break;
    } 

    //TODO: throw error
}

object Evaluator::op_unary_minus(object operand) {
    TokenType op = operand.type;

    switch(op) {
        case TokenType::INT:
            return object(-*static_cast<int *>(operand.getValue()));
        case TokenType::DOUBLE:
            return object(-*static_cast<double *>(operand.getValue()));
        case TokenType::IDENTIFIER:
            //TODO: implement
            break;
        default:
            break;
    } 
    
    //TODO: throw error
}

object Evaluator::op_not(object operand) {
    TokenType op = operand.type;

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

    //TODO: throw error
}
