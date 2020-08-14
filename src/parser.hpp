#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "expression.hpp"
#include "exceptions.hpp"
#include "error.hpp"
#include <vector>

class Parser {
    private:
        int cursor;
        std::vector<Token> tokens;
        std::string filename;
        std::string contents;
        std::unique_ptr<Error> currentError;


        //TODO: void addTernary(); add this later along with assignment
        ExprPtr parseExpression();
        ExprPtr addOr();
        ExprPtr addAnd();
        ExprPtr addEquality();
        ExprPtr addComparison();
        ExprPtr addAddition();
        ExprPtr addMultiplication();
        ExprPtr addPower();
        ExprPtr addPreUnary();
        ExprPtr addPostUnary();
        ExprPtr addOperand();

        bool matchesOperators(std::vector<TokenType> ops);
        void addError(std::string e_type, std::string e_desc);
        void handleError(); // this method allows parsing to continue even after an error has been found

    public:
        bool errorOccurred;
        std::vector<ExprPtr> expressions;

        Parser(std::vector<Token> t, std::string con, std::string file) 
        : tokens(t), filename(file), contents(con), errorOccurred(false), cursor(0) {}

        void parseTokens();
        void print(Expression *root, int space);
};

#endif