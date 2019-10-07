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
    size_t d;
    std::cout << std::stof(".1e10x0bq", &d) << std::endl;
    std::cout << d;
    //for (int i = 0; i < 30; i++)
    //    std::cout << w.getNextToken() << std::endl;
    return 0;
}