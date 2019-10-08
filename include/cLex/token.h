#ifndef CLEX_TOKEN_H
#define CLEX_TOKEN_H

#include <string>
#include <ostream>
#include <utility>
#include <cassert>

namespace cLex {
    // ISO C N2176 Section 6.4
    enum class TokenType {
#define TOKEN_TYPE(X, Y) X,
#include <def/TokenType.def>
#undef TOKEN_TYPE
    };

    // 6.4.1
    enum class KeyWord {
#define KEYWORD(X, Y) X,
#include <def/Keyword.def>
#undef KEYWORD
    };

    // 6.4.6
    enum class Punctuator {
#define PUNCTUATOR(X, Y) X,
#include <def/Punctuator.def>
#undef PUNCTUATOR
    };

    class TokenLocation {
    public:
        TokenLocation() : fileName{"UNKNOWN"}, colCount{0}, lineCount{1} {}
        TokenLocation(std::string file, size_t col, size_t line) :
        fileName{std::move(file)}, colCount{col}, lineCount{line} {}
        [[nodiscard]] size_t getColCount() const {
            return colCount;
        }
        [[nodiscard]] std::string toString() const {
            return fileName + ":" + std::to_string(lineCount) + ":" + std::to_string(colCount);
        }
    private:
        std::string fileName;
        size_t colCount;
        size_t lineCount;
    };

    class Token {
    public:
        Token(TokenType tokenType, TokenLocation tokenLocation, KeyWord keyWord, Punctuator punctuator,
              std::string literalValue)
                : tokenType_{tokenType}, tokenLocation_{std::move(tokenLocation)}, keyWord_{keyWord},
                  punctuator_{punctuator}, literalValue_{std::move(literalValue)} {

        }

        Token(TokenType tokenType, const TokenLocation& tokenLocation, KeyWord keyWord) :
                Token(tokenType, tokenLocation, keyWord, Punctuator::None, ""){
            assert(tokenType == TokenType::Keyword);
        }

        Token(TokenType tokenType, TokenLocation tokenLocation, Punctuator punctuator) :
                Token(tokenType, std::move(tokenLocation), KeyWord::None, punctuator, "") {
            assert(tokenType == TokenType::Punctuator);
        }

        Token(TokenType tokenType, TokenLocation tokenLocation, std::string literalValue) :
                Token(tokenType, std::move(tokenLocation), KeyWord::None, Punctuator::None, std::move(literalValue)){
            assert(tokenType != TokenType::Keyword && tokenType != TokenType::Punctuator);
        }

        Token() : Token(TokenType::Unknown, TokenLocation(), KeyWord::None, Punctuator::None, "") {}

        [[nodiscard]] TokenType getTokenType() const {
            return tokenType_;
        }

        [[nodiscard]] TokenLocation getTokenLocation() const {
            return tokenLocation_;
        }

        [[nodiscard]] KeyWord getKeyWord() const {
            return keyWord_;
        }

        [[nodiscard]] Punctuator getPunctuator() const {
            return punctuator_;
        }

        [[nodiscard]] std::string getLiteralValue() const {
            return literalValue_;
        }

        [[nodiscard]] std::string toString() const;
        // For debug.
        friend std::ostream& operator<<(std::ostream& os, const Token & token);
    private:
        TokenType tokenType_;
        TokenLocation tokenLocation_;

        KeyWord keyWord_;
        Punctuator punctuator_;
        std::string  literalValue_;
    };
}

#endif //CLEX_TOKEN_H
