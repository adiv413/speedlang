#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>

class Error {
    public:
        std::string error_type;
        std::string error_line; // the entire line on which the error falls
        std::string error_desc;
        std::string filename;
        int line;
        int col; // column (distance from the beginning of the line to the current position) of the beginning of the erroneous token

        Error(std::string type, std::string e_line, std::string desc, std::string file, int line_num, int col_num)
        : error_type(type), error_line(e_line), error_desc(desc), line(line_num), col(col_num), filename(file) {}

        void print();
};

#endif