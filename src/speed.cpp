#include <iostream>
#include <fstream>
#include <string>
#include "scanner.hpp"
#include "parser.hpp"
#include "evaluator.hpp"

using namespace std;

void runREPL();
void runFile(char *filename);
string get_file_contents(char *filename);
void printResult(object *o);

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
    cout << "Vitesse Language REPL\nVitesse v0.0.1\nType \"exit\" to exit.\n";
    string line = "";
    string filename = "REPL";

    while(true) {
        cout << ">>>"; 
        getline(cin, line);
        line += '\n';

        if(line == "exit") {
            break;
        }
        
        Scanner sc(filename, line);
        vector<Token> tokens = sc.scan_file_contents();

        for(auto token : tokens) {
            cout << token.value << " ";
        }

        cout << "\n\n\n";

        if(tokens.size() != 0) {
            Parser p(tokens, line, filename);
            p.parseTokens();
            if(!p.errorOccurred) {
                // for(int i = 0; i < p.statements.size(); i++) {
                //     p.print(static_cast<ExprStmt *>(&p.statements[i])->expression.get(), 0);
                // }

                Evaluator e(&p.statements, filename, line);
                vector<object> results = e.evaluate();

                for(int i = 0; i < results.size(); i++) {
                    object *o = &results[i];

                    printResult(o);
                }
            }
        }
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
        contents += '\n';
        Scanner sc(string(filename), contents);
        vector<Token> tokens = sc.scan_file_contents();

        // for(auto token : tokens) {
        //     cout << token.value << " ";
        // }

        // cout << "\n\n\n";

        if(tokens.size() != 0) {
            Parser p(tokens, contents, filename);
            p.parseTokens();
            if(!p.errorOccurred) {
                // for(int i = 0; i < p.statements.size(); i++) {
                //     p.print(p.statements[i].get(), 0);
                // }

                Evaluator e(&p.statements, filename, contents);
                vector<object> results = e.evaluate();

                for(int i = 0; i < results.size(); i++) {
                    object *o = &results[i];

                    printResult(o);
                }
            }
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

void printResult(object *o) {
    switch(o->type) {
        case TokenType::INT:
            cout << *static_cast<ll *>(o->getValue()) << "\n";
            break;
        case TokenType::DOUBLE:
            cout << *static_cast<ld *>(o->getValue()) << "\n";
            break;
        case TokenType::STRING:
            cout << *static_cast<string *>(o->getValue()) << "\n";
            break;
        case TokenType::TRUE:
            cout << "true" << "\n";
            break;
        case TokenType::FALSE:
            cout << "false" << "\n";
            break;
        case TokenType::NULL_T:
            cout << "null" << "\n";
            break;
    }
}