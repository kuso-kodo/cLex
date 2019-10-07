#include <iostream>
#include <fstream>
#include <cLex/file.h>
#include <cLex/token.h>

using namespace cLex;

int main(int argc, char** argv) {
    std::ifstream ifile;
    ifile.open("test.txt");
    auto w = FileWrapper("test.txt");
    while(!w.eof()) {

        std::cout << "ss" << w.eof() << "ss" << w.getNextChar() << "\n";
    }
    auto token = Token(TokenType::Punctuator, TokenLocation(), Punctuator::AmpAmp);
    std::cout << token << std::endl;
    return 0;
}