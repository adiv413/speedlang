#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <exception>
#include <string>

class FILE_NOT_FOUND_EXCEPTION : public std::exception {
    private:
        std::string message;
    public:
        FILE_NOT_FOUND_EXCEPTION(std::string filename) 
        : message("File \"" + filename + "\" not found") {}

        virtual const char* what() const throw()
        {
            return this->message.c_str();
        }
};

#endif