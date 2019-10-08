#include <cLex/token.h>

namespace cLex {
    const char *toCStyleString(TokenType tokenType) {
        switch (tokenType) {
#define TOKEN_TYPE(X, Y) case TokenType::X: return Y;
#include <def/TokenType.def>
            default:
                return "Unknown";
        }
    }

    const char *toCStyleString(KeyWord keyWord) {
        switch (keyWord) {
#define KEYWORD(X, Y) case KeyWord::X: return Y;
#include <def/Keyword.def>
            default:
                return "Unknown";
        }
    }

    const char *toCStyleString(Punctuator punctuator) {
        switch (punctuator) {
#define PUNCTUATOR(X, Y) case Punctuator::X: return Y;
#include <def/Punctuator.def>
            default:
                return "Unknown";
        }
    }

    std::string Token::toString() const {
        std::string result;
        result += "Type: ";
        result += std::string(toCStyleString(this->tokenType_)) + "\n";
        result += "Location: " + this->tokenLocation_.toString() + "\n";
        result += "Value: ";
        switch(this->tokenType_) {
            case TokenType::Keyword:
                result += std::string(toCStyleString(this->keyWord_));
                break;
            case TokenType::Punctuator:
                result += std::string(toCStyleString(this->punctuator_));
                break;
            case TokenType::EndOfFile:
                break;
            default:
                result += this->literalValue_;
                break;
        }
        result += "\n";
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const Token & token) {
        os << token.toString();
        return os;
    }
}
