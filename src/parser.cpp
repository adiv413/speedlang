#include "parser.hpp"

//TODO: do evaluate() in expression.cpp

std::vector<Expression> Parser::parseTokens() { 
    while(cursor < tokens.size()) {
        try {
            expressions.push_back(parseExpression());
        }
        catch(const std::exception &e) {
            if(currentError) {
                currentError.get()->print();
                currentError = nullptr;
            }
            handleError();
        }
    }
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
        TokenType::FALSE, TokenType::NULL_T})) return Expression(nullptr, nullptr, &tokens[cursor++]);

    else if(matchesOperators({TokenType::LEFT_PAREN})) {
        Token *left_paren = &tokens[cursor];
        cursor++;
        Expression expr = parseExpression();
        
        if(!matchesOperators({TokenType::RIGHT_PAREN})) {
            addError("SyntaxError", "Expected ')' to close parenthetical expression");
            throw UNCLOSED_PARENTHETICAL_SYNTAX_ERROR();
        }
        cursor++;

        return Expression(nullptr, &expr, left_paren);
    }

    addError("SyntaxError", "Expected expression; current token cannot start an expression");
    throw EXPECTED_EXPRESSION_SYNTAX_ERROR();
    
    //return Expression(nullptr, nullptr, nullptr);
}

bool Parser::matchesOperators(std::vector<TokenType> ops) {
    if(cursor >= tokens.size()) return false;
    if(tokens[cursor].token_type == TokenType::BACKSLASH) cursor++;

    if(cursor < tokens.size()) {
        for(auto i : ops) {
            if(tokens[cursor].token_type == i) return true;
        }
    }
    
    return false;
}

void Parser::addError(std::string e_type, std::string e_desc) {
    Token t = tokens[cursor];
    int start = t.line_begin;
    int end = start;

    while(end < contents.size() && end != '\n') end++;

    std::string e_line(contents, start, end - start);

    currentError = std::unique_ptr<Error>(new Error(e_type, e_line, e_desc, filename, t.line_number, t.col_number));
    errorOccurred = true;
}

void Parser::handleError() {
    while(cursor < tokens.size()) {
        Token current = tokens[cursor];

        switch(current.token_type) {
            //TODO: handle these cases
            case TokenType::CLASS:
            case TokenType::FUNC:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::RETURN:

            case TokenType::NEWLINE:
            case TokenType::SEMICOLON:
                cursor++;
                break;
            default:
                cursor++;
        }
    }
}