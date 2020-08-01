#include "error.hpp"

void Error::print() {
    std::cout << error_type << " (line " << line << ", column " << col << " in file " << filename << "): " << error_desc << "\n";
    std::cout << "\t" << error_line << "\n";
    std::cout << "\t" << std::string(col, ' ') << "^" << "\n";
}
