#include "object.hpp"

object::object() {}

object::object(Token t) {
    type = t.token_type;
    switch(type) {
        case TokenType::INT:
            {
                int temp = std::stoi(t.value);
                type_ptr = std::make_unique<int_t>(temp);
                break;
            }
        case TokenType::DOUBLE:
            {
                double temp = std::stod(t.value);
                type_ptr = std::make_unique<double_t>(temp);
                break;
            }
        case TokenType::TRUE:
            {
                bool temp = true;
                type_ptr = std::make_unique<bool_t>(temp);
                break;
            }
        case TokenType::FALSE:
            {
                bool temp = false;
                type_ptr = std::make_unique<bool_t>(temp);
                break;
            }
        case TokenType::STRING:
            {
                type_ptr = std::make_unique<string_t>(t.value);
                break;
            }
        case TokenType::NULL_T:
            {
                type_ptr = nullptr;
                break;
            }
        default:
            break;
            //TODO: throw an error or smth
    }
}

void* object::getValue() {
    return type_ptr->getValue();
}

void* int_t::getValue() {
    return static_cast<void *>(&value);
}

void* bool_t::getValue() {
    return static_cast<void *>(&value);
}

void* double_t::getValue() {
    return static_cast<void *>(&value);
}

void* string_t::getValue() {
    return static_cast<void *>(&value);
}