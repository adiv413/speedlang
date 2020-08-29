#include "object.hpp"

object::object() {}

object::object(ll i) : type_ptr(new int_t(i)), type(TokenType::INT) {}

object::object(ld d) : type_ptr(new ldouble_t(d)), type(TokenType::DOUBLE) {}

object::object(bool b) : type_ptr(new bool_t(b)), type(b ? TokenType::TRUE : TokenType::FALSE) {}

object::object(std::string s) : type_ptr(new string_t(s)), type(TokenType::STRING) {}

object::object(std::nullptr_t n) : type_ptr(n), type(TokenType::NULL_T) {}

object::object(Token *t) {
    type = t->token_type;
    switch(type) {
        case TokenType::INT:
            {
                type_ptr = std::make_unique<int_t>(std::stoll(t->value));
                break;
            }
        case TokenType::DOUBLE:
            {
                type_ptr = std::make_unique<ldouble_t>(std::stold(t->value));
                break;
            }
        case TokenType::TRUE:
            {
                type_ptr = std::make_unique<bool_t>(true);
                break;
            }
        case TokenType::FALSE:
            {
                type_ptr = std::make_unique<bool_t>(false);
                break;
            }
        case TokenType::STRING:
            {
                type_ptr = std::make_unique<string_t>(t->value);
                break;
            }
        case TokenType::IDENTIFIER:
            {
                type_ptr = std::make_unique<reference_t>(t->value);
                break;
            }
        case TokenType::NULL_T:
            {
                type_ptr = nullptr;
                break;
            }
        default:
            break;
            //TODO: throw a BAD_ENUM_VALUE error or smth
    }
}

void* object::getValue() {
    if(type_ptr) return type_ptr->getValue();
    else return nullptr;
}

void* int_t::getValue() {
    return static_cast<void *>(&value);
}

void* bool_t::getValue() {
    return static_cast<void *>(&value);
}

void* ldouble_t::getValue() {
    return static_cast<void *>(&value);
}

void* string_t::getValue() {
    return static_cast<void *>(&value);
}

void* reference_t::getValue() {
    return static_cast<void *>(&identifier_name);
}