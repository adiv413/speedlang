#include "parser.hpp"

//TODO: do evaluate() in expression.cpp

void Parser::parseTokens() { 
    while(cursor < tokens.size()) {
        if(tokens[cursor].token_type == TokenType::NEWLINE || tokens[cursor].token_type == TokenType::SEMICOLON) cursor++;
        else {
            try {
                expressions.push_back(parseExpression());

                // this if statement is necessary because expressions like 5 + 3 - 2 2 - 3 are legal otherwise
                // those multiple-expressions-in-one-line expressions are evaluated as separate expressions 
                // (e.g. 5 + 3 - 2 and 2 - 3), and are dangerous because they cause ambiguity when dealing with
                // longer and more complex expressions
                
                // if(cursor < tokens.size() && tokens[cursor].token_type != TokenType::NEWLINE && 
                //     tokens[cursor].token_type != TokenType::SEMICOLON) {

                //     addError("SyntaxError", "Expected semicolon or newline after end of expression");
                //     throw MISSING_LINE_SEPARATOR_SYNTAX_ERROR();
                // }
            }
            catch(const std::exception &e) {
                if(currentError) {
                    currentError->print();
                    currentError = nullptr;
                }
                else {
                    addError("SyntaxError", "");
                    currentError->print();
                    currentError = nullptr;
                }
                handleError();
            }
        }
    }
}

ExprPtr Parser::parseExpression() {
    return std::move(addOr());
}

ExprPtr Parser::addOr() {
    ExprPtr expr(std::move(addXor()));

    while(matchesOperators({TokenType::OR})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addXor()));
        expr = std::make_unique<Expression>(std::move(expr), std::move(right), std::move(op));
    }

    return std::move(expr);
}

ExprPtr Parser::addXor() {
    ExprPtr expr(std::move(addAnd()));

    while(matchesOperators({TokenType::XOR})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addAnd()));
        expr = std::make_unique<Expression>(std::move(expr), std::move(right), std::move(op));
    }

    return std::move(expr);
}

ExprPtr Parser::addAnd() {
    ExprPtr expr(std::move(addEquality()));

    while(matchesOperators({TokenType::AND})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addEquality()));
        expr = std::make_unique<Expression>(std::move(expr), std::move(right), std::move(op));
    }

    return std::move(expr);
}

ExprPtr Parser::addEquality() {
    ExprPtr expr(std::move(addComparison()));

    while(matchesOperators({TokenType::EQUAL_EQUAL, TokenType::NOT_EQUAL})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addComparison()));
        expr = std::make_unique<Expression>(std::move(expr), std::move(right), std::move(op));
    }

    return std::move(expr);
}

ExprPtr Parser::addComparison() {
    ExprPtr expr(std::move(addAddition()));

    while(matchesOperators({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addAddition()));
        expr = std::make_unique<Expression>(std::move(expr), std::move(right), std::move(op));
    }

    return std::move(expr);
}

ExprPtr Parser::addAddition() {
    ExprPtr expr(std::move(addMultiplication()));

    while(matchesOperators({TokenType::PLUS, TokenType::MINUS})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addMultiplication()));
        expr = std::make_unique<Expression>(std::move(expr), std::move(right), std::move(op));
    }

    return std::move(expr);
}

ExprPtr Parser::addMultiplication() {
    ExprPtr expr(std::move(addPower()));

    while(matchesOperators({TokenType::STAR, TokenType::SLASH, TokenType::PERCENT})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addPower()));
        expr = std::make_unique<Expression>(std::move(expr), std::move(right), std::move(op));
    }

    return std::move(expr);
}

ExprPtr Parser::addPower() {
    ExprPtr expr(std::move(addPreUnary()));

    while(matchesOperators({TokenType::CARET})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addPreUnary()));
        expr = std::make_unique<Expression>(std::move(expr), std::move(right), std::move(op));
    }

    return std::move(expr);
}

ExprPtr Parser::addPreUnary() {
    if(matchesOperators({TokenType::PLUS_PLUS, TokenType::MINUS_MINUS, TokenType::NOT, TokenType::PLUS, TokenType::MINUS})) {
        TokenPtr op(new Token(tokens[cursor++]));
        ExprPtr right(std::move(addPreUnary()));
        return std::make_unique<Expression>(nullptr, std::move(right), std::move(op));
    }

    return std::move(addPostUnary());
}

ExprPtr Parser::addPostUnary() {
    //if current is a operand and next is a ++ or --

    if(matchesOperators({TokenType::INT, TokenType::DOUBLE, TokenType::STRING, TokenType::IDENTIFIER, TokenType::TRUE, 
        TokenType::FALSE, TokenType::NULL_T}) 
        && (tokens[cursor + 1].token_type == TokenType::PLUS_PLUS || tokens[cursor + 1].token_type == TokenType::MINUS_MINUS)) {
        
        ExprPtr left(std::move(addOperand())); // addOperand increments cursor for us so we can just call tokens[cursor] for the op
        TokenPtr op(new Token(tokens[cursor]));

        cursor++;

        return std::make_unique<Expression>(std::move(left), nullptr, std::move(op));
    }

    return std::move(addOperand());
}

ExprPtr Parser::addOperand() {
    if(matchesOperators({TokenType::INT, TokenType::DOUBLE, TokenType::STRING, TokenType::IDENTIFIER, TokenType::TRUE, 
        TokenType::FALSE, TokenType::NULL_T})) {

        return std::make_unique<Expression>(ExprPtr(), ExprPtr(), TokenPtr(new Token(tokens[cursor++])));
    }

    else if(matchesOperators({TokenType::LEFT_PAREN})) {
        TokenPtr left_paren(new Token(tokens[cursor++]));
        ExprPtr expr(std::move(parseExpression()));
        
        if(!matchesOperators({TokenType::RIGHT_PAREN})) {
            addError("SyntaxError", "Expected ')' to close parenthetical expression");
            throw UNCLOSED_PARENTHETICAL_SYNTAX_ERROR();
        }
        cursor++;

        return std::make_unique<Expression>(nullptr, std::move(expr), std::move(left_paren));
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


void Parser::print(Expression *root, int space)  
{  
    int COUNT = 10;
    // Base case  
    if (root == NULL)  
        return;  
  
    // Increase distance between levels  
    space += COUNT;  
  
    // Process right child first  
    print(root->right.get(), space);  
  
    // Print current node after space  
    // count  
    std::cout<<std::endl;  
    for (int i = COUNT; i < space; i++)  
        std::cout<<" ";  
    std::cout<<root->op->value<<"\n";  
  
    // Process left child  
    print(root->left.get(), space);  
}  
