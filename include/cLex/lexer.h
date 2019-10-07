#ifndef CLEX_LEXER_H
#define CLEX_LEXER_H

#include <string>
#include <cLex/token.h>
#include <cLex/file.h>
#include <vector>

namespace cLex {
    class Lexer {
    public:
        explicit Lexer(const std::string& fileName) : fileWrapper_{fileName} {}
        [[nodiscard]] Token getToken() const {
            return token_;
        }

        Token getNextToken();
    private:
        void skipLineComment();
        void skipBlockComment();
        Token getNextNumericToken();
        Token getNextStringLiterialToken();
        Token getNextCharacterConstantToken();
        FileWrapper fileWrapper_;
        TokenLocation currentLocation_;
        Token token_;
        std::string tokenBuffer_;
    };
}

#endif //CLEX_LEXER_H
