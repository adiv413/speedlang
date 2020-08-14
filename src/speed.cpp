#include <iostream>
#include <fstream>
#include <string>
#include "scanner.hpp"
#include "parser.hpp"

using namespace std;

void runREPL();
void runFile(char *filename);
string get_file_contents(char *filename);

int main(int argc, char **argv) {
    if(argc > 2) {
        cerr << "Usage: speed [file]";
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
    string contents;
    bool exception_caught = false;

    try {
        contents = get_file_contents(filename);
    }
    catch(const exception &e) {
        exception_caught = true;
        cerr << "ERROR: " << e.what() << "\n";
    }

    if(!exception_caught) {
        Scanner sc(string(filename), contents);
        vector<Token> tokens = sc.scan_file_contents();

        for(auto token : tokens) {
            cout << token.value << " ";
        }

        cout << "\n\n\n";

        if(tokens.size() != 0) {
            Parser p(tokens, contents, filename);
            p.parseTokens();
            if(!p.errorOccurred) {
                for(int i = 0; i < p.expressions.size(); i++) {
                    p.print(p.expressions[i].get(), 0);
                }
            }
            //parse_file_contents - syntax analysis
            //execute_code
        }
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