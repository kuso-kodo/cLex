#include <iostream>
#include <fstream>
#include <cLex/file.h>
#include <cLex/token.h>
#include <cLex/lexer.h>
#include <cLex/counter.h>
#include <cstdlib>
#include <string>

using namespace cLex;

void usage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "cLex <file name>" << std::endl;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        usage();
        return 0;
    }
    auto counter = Counter();
    auto lexer = Lexer(argv[1]);
    std::cout << "Tokens:" << std::endl;
    Token tok = lexer.getNextToken();
    while(tok.getTokenType() != TokenType::EndOfFile) {
        std::cout << tok << std::endl;
        counter.update(tok);
        tok = lexer.getNextToken();
    }
    std::cout << "File info:" << std::endl;
    std::cout << "File name:\t" << lexer.getSrcName() << std::endl;
    std::cout << "Total chars:\t" << lexer.getCount() << std::endl;
    std::cout << "Total lines:\t" << lexer.getLineCount() << std::endl;
    std::cout << counter;
    return 0;
}