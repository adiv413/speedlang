#include "parser.hpp"

//TODO: do evaluate() in expression.cpp

std::vector<Expression> Parser::parseTokens() { //TODO: wrap in a try catch to catch parseerrors
    expressions.push_back(parseExpression());
    return expressions;
}

Expression Parser::parseExpression() {
    return addOr();
}

Expression Parser::addOr() {
    Expression expr = addAnd();

    while(matchesOperators({TokenType::OR})) {
        Token *op = &tokens[cursor++];
        Expression right = addAnd();
        expr = Expression(&expr, &right, op);
    }

    return expr;
}

Expression Parser::addAnd() {
    Expression expr = addEquality();

    while(matchesOperators({TokenType::AND})) {
        Token *op = &tokens[cursor++];
        Expression right = addEquality();
        expr = Expression(&expr, &right, op);
    }

    return expr;
}

Expression Parser::addEquality() {
    Expression expr = addComparison();

    while(matchesOperators({TokenType::EQUAL_EQUAL, TokenType::NOT_EQUAL})) {
        Token *op = &tokens[cursor++];
        Expression right = addComparison();
        expr = Expression(&expr, &right, op);
    }

    return expr;
}

Expression Parser::addComparison() {
    Expression expr = addAddition();

    while(matchesOperators({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token *op = &tokens[cursor++];
        Expression right = addAddition();
        expr = Expression(&expr, &right, op);
    }

    return expr;
}

Expression Parser::addAddition() {
    Expression expr = addMultiplication();

    while(matchesOperators({TokenType::PLUS, TokenType::MINUS})) {
        Token *op = &tokens[cursor++];
        Expression right = addMultiplication();
        expr = Expression(&expr, &right, op);
    }

    return expr;
}

Expression Parser::addMultiplication() {
    Expression expr = addPower();

    while(matchesOperators({TokenType::STAR, TokenType::SLASH, TokenType::PERCENT})) {
        Token *op = &tokens[cursor++];
        Expression right = addPower();
        expr = Expression(&expr, &right, op);
    }

    return expr;
}

Expression Parser::addPower() {
    Expression expr = addPreUnary();

    while(matchesOperators({TokenType::CARET})) {
        Token *op = &tokens[cursor++];
        Expression right = addPreUnary();
        expr = Expression(&expr, &right, op);
    }

    return expr;
}

Expression Parser::addPreUnary() {
    if(matchesOperators({TokenType::PLUS_PLUS, TokenType::MINUS_MINUS, TokenType::NOT, TokenType::PLUS, TokenType::MINUS})) {
        Token *op = &tokens[cursor++];
        Expression right = addPreUnary();
        return Expression(nullptr, &right, op);
    }

    return addPostUnary();
}

Expression Parser::addPostUnary() {
    //if current is a operand and next is a ++ or --

    if(matchesOperators({TokenType::INT, TokenType::DOUBLE, TokenType::STRING, TokenType::IDENTIFIER, TokenType::TRUE, 
        TokenType::FALSE, TokenType::NULL_T}) 
        && (tokens[cursor + 1].token_type == TokenType::PLUS_PLUS || tokens[cursor + 1].token_type == TokenType::MINUS_MINUS)) {
        
        Expression left = addOperand();
        Token *op = &tokens[cursor + 1];
        cursor += 2;

        return Expression(&left, nullptr, op);
    }

    return addOperand();
}

Expression Parser::addOperand() {
    if(matchesOperators({TokenType::INT, TokenType::DOUBLE, TokenType::STRING, TokenType::IDENTIFIER, TokenType::TRUE, 
        TokenType::FALSE, TokenType::NULL_T})) {
        return Expression(nullptr, nullptr, &tokens[cursor++]);
    } 
    else if(matchesOperators({TokenType::LEFT_PAREN})) {
        Token *left_paren = &tokens[cursor];
        cursor++;
        Expression expr = parseExpression();
        
        if(!matchesOperators({TokenType::RIGHT_PAREN})) {
            //TODO: throw error: unclosed parenthetical expression
        }
        cursor++;

        return Expression(nullptr, &expr, left_paren);
    }


    //TODO: else throw error: expected expression
    return Expression(nullptr, nullptr, nullptr);
}

bool Parser::matchesOperators(std::vector<TokenType> ops) {
    if(cursor >= tokens.size()) return false;

    for(auto i : ops) {
        if(tokens[cursor].token_type == i) return true;
    }
    return false;
}
