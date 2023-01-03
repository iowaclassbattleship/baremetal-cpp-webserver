#include <fstream>
#include <string>
#include <iostream>

std::string get_file_as_string(std::string& filename) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << filename << "'" << "\n";
        exit(1);
    }

    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>()) + "\n\0";
}