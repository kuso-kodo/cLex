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

        [[nodiscard]] size_t getCount() const {
            return fileWrapper_.getCount();
        }

        [[nodiscard]] size_t getLineCount() const {
            return fileWrapper_.getLineCount();
        }

        [[nodiscard]] std::string getSrcName() const {
            return fileWrapper_.getName();
        }
    private:
        void skipLineComment();
        void skipBlockComment();
        Token getNextNumericToken();
        Token getNextStringLiterialToken();
        Token getNextCharacterConstantToken();
        Token getNextIdentifierToken();
        Token getNextPreprocessingDirectiveToken();
        FileWrapper fileWrapper_;
        TokenLocation currentLocation_;
        Token token_;
        std::string tokenBuffer_;
    };
}

#endif //CLEX_LEXER_H
