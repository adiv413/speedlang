#include "object.hpp"

object::object(Token t) {
    type = t.token_type;
    switch(type) {
        case TokenType::INT:
            {
                int temp = std::stoi(t.value);
                value = static_cast<void *>(&temp);
                break;
            }
        case TokenType::DOUBLE:
            {
                double temp = std::stod(t.value);
                value = static_cast<void *>(&temp);
                break;
            }
        case TokenType::TRUE:
            {
                bool temp = true;
                value = static_cast<void *>(&temp);
                break;
            }
        case TokenType::FALSE:
            {
                bool temp = false;
                value = static_cast<void *>(&temp);
                break;
            }
        case TokenType::NULL_T:
            {
                value = nullptr;
                break;
            }
        case TokenType::STRING:
            {
                value = static_cast<void *>(&t.value);
            break;
            }
    }
}