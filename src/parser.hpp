#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "expression.hpp"
#include <vector>

class Parser {
    private:
        int cursor;
        std::vector<Token> tokens;
        std::vector<Expression> expressions;
        std::string filename;
        std::string contents;
        bool errorOccurred;

        //void addTernary(); add this later along with equality
        Expression parseExpression();
        Expression addOr();
        Expression addAnd();
        Expression addEquality();
        Expression addComparison();
        Expression addAddition();
        Expression addMultiplication();
        Expression addPower();
        Expression addPreUnary();
        Expression addPostUnary();
        Expression addOperand();

        bool matchesOperators(std::vector<TokenType> ops);

        // at lowest precedence, left = addoperatorhigherprecedence, if(match(operator)) operator = operator and right = w/e
        // this recurses all the way up and brings it back all the way

    public:
        Parser(std::vector<Token> t, std::string con, std::string file, bool error) 
        : tokens(t), filename(file), contents(con), errorOccurred(error), cursor(0) {}

        std::vector<Expression> parseTokens();
};

#endif