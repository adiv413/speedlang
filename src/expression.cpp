#include "expression.hpp"

Expression::~Expression() {
    delete left;
    delete right;
    delete op;
}