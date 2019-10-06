#include <iostream>
#include <cLex/token.h>

using namespace cLex;

int main(int argc, char** argv) {
    auto token = Token(TokenType::Punctuator, TokenLocation(), Punctuator::AmpAmp);
    std::cout << token << std::endl;
    return 0;
}