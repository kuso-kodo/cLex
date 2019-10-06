#include <iostream>
#include <fstream>
#include <cLex/file.h>
#include <cLex/token.h>

using namespace cLex;

int main(int argc, char** argv) {
    std::ifstream ifile;
    ifile.open("test.txt");
    auto w = FileWrapper(ifile);
    std::cout << w.getNextChar();
    std::cout << w.peekChar(11) << std::endl;
    std::cout << w.getNextChar();
    auto token = Token(TokenType::Punctuator, TokenLocation(), Punctuator::AmpAmp);
    std::cout << token << std::endl;
    return 0;
}