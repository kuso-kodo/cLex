#include <iostream>
#include <fstream>
#include <cLex/file.h>
#include <cLex/token.h>
#include <cLex/lexer.h>
#include <cstdlib>
#include <string>

using namespace cLex;

int main(int argc, char** argv) {
    auto w = Lexer("test.txt");
    for (int i = 0; i < 4; i++)
        std::cout << w.getNextToken() << std::endl;
    return 0;
}