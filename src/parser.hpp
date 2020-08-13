#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "expression.hpp"
#include "exceptions.hpp"
#include "error.hpp"
#include <vector>
#include <memory>

class Parser {
    private:
        int cursor;
        std::vector<Token> tokens;
        std::vector<Expression> expressions;
        std::string filename;
        std::string contents;
        std::unique_ptr<Error> currentError;


        //TODO: void addTernary(); add this later along with assignment
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
        void addError(std::string e_type, std::string e_desc);
        void handleError(); // this method allows parsing to continue even after an error has been found

    public:
        bool errorOccurred;

        Parser(std::vector<Token> t, std::string con, std::string file) 
        : tokens(t), filename(file), contents(con), errorOccurred(false), cursor(0) {}

        std::vector<Expression> parseTokens();
};

#endif