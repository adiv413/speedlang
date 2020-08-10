#include <iostream>
#include <fstream>
#include <string>
#include "scanner.hpp"

using namespace std;

void runREPL();
void runFile(char *filename);
string get_file_contents(char *filename);

int main(int argc, char **argv) {
    if(argc > 2) {
        cout << "Usage: speed [file]";
        return 64; // exit code 64: incorrect command usage
    }
    else if(argc == 2) {
        runFile(argv[1]);
    }
    else {
        runREPL();
    }

    return 0;
}

void runREPL() {
    string x = "";

    while(x != "exit") {
        cout << ">>>"; 
        cin >> x;
        cout << x << "\n";
    }

}

void runFile(char *filename) {
    string contents = get_file_contents(filename);
    Scanner sc(string(filename), contents);
    vector<Token> tokens = sc.scan_file_contents();

    for(auto token : tokens) {
        cout << token.value << "\n";
    }

    if(tokens.size() != 0) {
        //parse_file_contents - syntax analysis
        //execute_code
    }
}

string get_file_contents(char *filename) {
    ifstream in;

    try {
        in = ifstream(filename, ios::in | ios::binary);
    }
    catch(...) {
        throw FILE_NOT_FOUND_EXCEPTION(filename);
    }

    if(in) {
        string contents;
        in.seekg(0, ios::end);
        contents.resize(in.tellg());
        in.seekg(0, ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    else {
        throw FILE_NOT_FOUND_EXCEPTION(string(filename));
    }

    return ""; 
}