#include <cLex/lexer.h>

#define GOT_TOKEN(TYPE, VALUE) return Token(TYPE, currentLocation_, VALUE)
#define GOT_PUNCTOATOR(X) GOT_TOKEN(TokenType::Punctuator, X)
#define GOT_KEYWORD(X) GOT_TOKEN(TokenType::Keyword, X)

namespace cLex {

    Token Lexer::getNextToken() {
        tokenBuffer_.clear();
        if(fileWrapper_.eof())
            return Token(TokenType::EndOfFile, fileWrapper_.getLocation(), "");
        auto c = fileWrapper_.getNextChar();
        while(isblank(c) || c == '\n') {
            if(fileWrapper_.eof())
                return Token(TokenType::EndOfFile, fileWrapper_.getLocation(), "");
            c = fileWrapper_.getNextChar();
        }
        currentLocation_ = fileWrapper_.getLocation();
        switch (c) {
            // Punctuators
            case '[':
                GOT_PUNCTOATOR(Punctuator::LeftSquare);
            case ']':
                GOT_PUNCTOATOR(Punctuator::RightSquare);
            case '(':
                GOT_PUNCTOATOR(Punctuator::LeftParen);
            case ')':
                GOT_PUNCTOATOR(Punctuator::RightParen);
            case '{':
                GOT_PUNCTOATOR(Punctuator::LeftBrace);
            case '}':
                GOT_PUNCTOATOR(Punctuator::RightBrace);
            case '.': {
                if(fileWrapper_.peekChar(1) == '.' && fileWrapper_.peekChar(2) == '.') {
                    fileWrapper_.eatChars(2);
                    GOT_PUNCTOATOR(Punctuator::Ellipsis);
                } else if(isdigit(fileWrapper_.peekChar(1))) {
                    tokenBuffer_.push_back(c);
                    return getNextNumericToken();
                } else
                    GOT_PUNCTOATOR(Punctuator::Period);
            }
            case '-': {
                switch (fileWrapper_.peekChar(1)) {
                    case '>':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::Arrow);
                    case '-':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::MinusMinus);
                    case '=':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::MinusEqual);
                    default:
                        GOT_PUNCTOATOR(Punctuator::Minus);
                }
            }
            case '+': {
                switch (fileWrapper_.peekChar(1)) {
                    case '+':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::PlusPlus);
                    case '=':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::PlusEqual);
                    default:
                        GOT_PUNCTOATOR(Punctuator::Plus);
                }
            }
            case '&': {
                switch (fileWrapper_.peekChar(1)) {
                    case '&':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::AmpAmp);
                    case '=':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::AmpEqual);
                    default:
                        GOT_PUNCTOATOR(Punctuator::Amp);
                }
            }
            case '*':
                if(fileWrapper_.peekChar(1) == '=') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::StarEqual);
                } else
                    GOT_PUNCTOATOR(Punctuator::Star);
            case '~':
                GOT_PUNCTOATOR(Punctuator::Tilde);
            case '!':
                if(fileWrapper_.peekChar(1) == '=') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::ExclaimEqual);
                } else
                    GOT_PUNCTOATOR(Punctuator::Exclaim);
            case '/':
                if(fileWrapper_.peekChar(1) == '/') {
                    fileWrapper_.eatChars(1);
                    skipLineComment();
                    return getNextToken();
                } else if(fileWrapper_.peekChar(1) == '*') {
                    fileWrapper_.eatChars(1);
                    skipBlockComment();
                    return getNextToken();
                } else if(fileWrapper_.peekChar(1) == '=') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::SlashEqual);
                }else
                    GOT_PUNCTOATOR(Punctuator::Slash);
            case '%':
                if(fileWrapper_.peekChar(1) == '=') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::PercentEqual);
                } else if(fileWrapper_.peekChar(1) == '>') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::RightBrace);
                } else if(fileWrapper_.peekChar(1) == ':') {
                    if(fileWrapper_.peekChar(2) == '%' && fileWrapper_.peekChar(3) == ':') {
                        fileWrapper_.eatChars(3);
                        GOT_PUNCTOATOR(Punctuator::HashHash);
                    } else {
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::Hash);
                    }
                } else
                    GOT_PUNCTOATOR(Punctuator::Percent);
            case '<':
                if(fileWrapper_.peekChar(1) == '<') {
                    if(fileWrapper_.peekChar(2) == '=') {
                        fileWrapper_.eatChars(2);
                        GOT_PUNCTOATOR(Punctuator::LessLessEqual);
                    } else {
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::LessLess);
                    }
                } else if(fileWrapper_.peekChar(1) == '=') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::LessEqual);
                } else if(fileWrapper_.peekChar(1) == ':') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::LeftSquare);
                } else if(fileWrapper_.peekChar(1) == '%') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::LeftBrace);
                } else
                    GOT_PUNCTOATOR(Punctuator::Less);
            case '>':
                if(fileWrapper_.peekChar(1) == '>') {
                    if(fileWrapper_.peekChar(2) == '=') {
                        fileWrapper_.eatChars(2);
                        GOT_PUNCTOATOR(Punctuator::GreaterGreaterEqual);
                    } else if(fileWrapper_.peekChar(1) == '=') {
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::GreaterEqual);
                    } else {
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::GreaterGreater);
                    }
                } else
                    GOT_PUNCTOATOR(Punctuator::GreaterGreater);
            case '=':
                if(fileWrapper_.peekChar(1) == '=') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::EqualEqual);
                } else
                    GOT_PUNCTOATOR(Punctuator::Equal);
            case '^':
                if(fileWrapper_.peekChar(1) == '=') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::CaretEqual);
                } else
                    GOT_PUNCTOATOR(Punctuator::Caret);
            case '|': {
                switch (fileWrapper_.peekChar(1)) {
                    case '|':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::PipePipe);
                    case '=':
                        fileWrapper_.eatChars(1);
                        GOT_PUNCTOATOR(Punctuator::PipeEqual);
                    default:
                        GOT_PUNCTOATOR(Punctuator::Pipe);
                }
            }
            case '?':
                GOT_PUNCTOATOR(Punctuator::Question);
            case ':':
                if(fileWrapper_.peekChar(1) == '>') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::RightSquare);
                } else
                    GOT_PUNCTOATOR(Punctuator::Colon);
            case ';':
                GOT_PUNCTOATOR(Punctuator::Semi);
            case ',':
                GOT_PUNCTOATOR(Punctuator::Comma);
            case '#':
                if(fileWrapper_.peekChar(1) == '#') {
                    fileWrapper_.eatChars(1);
                    GOT_PUNCTOATOR(Punctuator::HashHash);
                } else if(currentLocation_.getColCount() == 1) {
                    return getNextPreprocessingDirectiveToken();
                } else
                    GOT_PUNCTOATOR(Punctuator::Hash);
            case '\"':
                return getNextStringLiterialToken();
            case '\'':
                return getNextCharacterConstantToken();
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                tokenBuffer_.push_back(c);
                return getNextNumericToken();
            case 'L':
            case 'U':
                if(fileWrapper_.peekChar(1) == '\"') {
                    fileWrapper_.eatChars(1);
                    return getNextStringLiterialToken();
                } else if(fileWrapper_.peekChar(1) == '\'') {
                    fileWrapper_.eatChars(1);
                    return getNextCharacterConstantToken();
                } else {
                    tokenBuffer_.push_back(c);
                    return getNextIdentifierToken();
                }
            case 'u':
                if(fileWrapper_.peekChar(1) == '\"') {
                    fileWrapper_.eatChars(1);
                    return getNextStringLiterialToken();
                } else if(fileWrapper_.peekChar(1) == '\'') {
                    fileWrapper_.eatChars(1);
                    return getNextCharacterConstantToken();
                } else if((fileWrapper_.peekChar(1) == '8') && fileWrapper_.peekChar(2) == '\"') {
                    fileWrapper_.eatChars(2);
                    return getNextStringLiterialToken();
                } else {
                    tokenBuffer_.push_back(c);
                    return getNextIdentifierToken();
                }
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
            case 'H': case 'I': case 'J': case 'K': /*'L'*/   case 'M': case 'N':
            case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':    /*'U'*/
            case 'V': case 'W': case 'X': case 'Y': case 'Z':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
            case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
            case 'o': case 'p': case 'q': case 'r': case 's': case 't':    /*'u'*/
            case 'v': case 'w': case 'x': case 'y': case 'z':
            case '_':
                tokenBuffer_.push_back(c);
                return getNextIdentifierToken();
            default:
                tokenBuffer_.push_back(c);
                break;
        }
        auto result = Token(TokenType::Unknown, fileWrapper_.getLocation(), tokenBuffer_);
        return result;
    }

    Token Lexer::getNextPreprocessingDirectiveToken() {
        char c = '*';
        while(!fileWrapper_.eof() && (fileWrapper_.peekChar(1) != '\n' || c == '\\')) {
            c = fileWrapper_.getNextChar();
            tokenBuffer_.push_back(c);
        }
        return Token(TokenType::PreprocessingDirective, currentLocation_, tokenBuffer_);
    }

    void Lexer::skipLineComment() {
        while(!fileWrapper_.eof() && fileWrapper_.getNextChar() != '\n');
    }

    void Lexer::skipBlockComment() {
        while(!fileWrapper_.eof()) {
            if(fileWrapper_.getNextChar() == '*' && fileWrapper_.peekChar(1) == '/') {
                fileWrapper_.eatChars(1);
                return;
            }
        }
    }

    bool isPermittedNumericToken(const char c) {
        return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ||
                (c == 'x') || (c == 'X') || (c == 'p') || (c == 'P') || (c == 'l') || (c == 'L') || (c == 'u') || (c == 'U') || (c == '.');
    }

    bool isPermittedNumericSuffix(const char c) {
        return c == 'f' || c == 'F' || c == 'L' || c == 'l';
    }

    bool isPermittedNumericSuffix(const char c1, const char c2) {
        return (c1 == 'L' && c2 == 'L') || (c1 == 'l' && c2 == 'l') || ((c1 == 'u' || c1 == 'U') && (c2 == 'L' || c2 == 'l'));
    }

    bool isPermittedNumericSuffix(const char c1, const char c2, const char c3) {
        return ((c1 == 'u' || c1 == 'U') && ((c2 == 'L' && c3 == 'L') || (c2 == 'l' && c3 == 'l')));
    }

    Token Lexer::getNextNumericToken() {
        while(!fileWrapper_.eof() && isPermittedNumericToken(fileWrapper_.peekChar(1))) {
            if((fileWrapper_.peekChar(1) == 'e' || fileWrapper_.peekChar(1) == 'E' ||
                    fileWrapper_.peekChar(1) == 'p' || fileWrapper_.peekChar(1) == 'P') &&
                    (fileWrapper_.peekChar(2) == '+' || fileWrapper_.peekChar(2) == '-')) {
                tokenBuffer_.push_back(fileWrapper_.getNextChar());
            }
            tokenBuffer_.push_back(fileWrapper_.getNextChar());
        }
        size_t offset;
        std::stof(tokenBuffer_, &offset);
        if(offset == tokenBuffer_.size() ||
            (offset + 1 == tokenBuffer_.size() && isPermittedNumericSuffix(tokenBuffer_.at(offset))) ||
                (offset + 2 == tokenBuffer_.size() && isPermittedNumericSuffix(tokenBuffer_.at(offset), tokenBuffer_.at(offset + 1))) ||
                        (offset + 3 == tokenBuffer_.size() && isPermittedNumericSuffix(tokenBuffer_.at(offset), tokenBuffer_.at(offset + 1), tokenBuffer_.at(offset + 2)))) {
            return Token(TokenType::NumericConstant, currentLocation_, tokenBuffer_);
        }
        return Token(TokenType::NumericConstantWithError, currentLocation_, tokenBuffer_);
    }

    Token Lexer::getNextStringLiterialToken() {
        char c;
        while(!fileWrapper_.eof() && (c = fileWrapper_.getNextChar()) != '\"') {
            tokenBuffer_.push_back(c);
        }
        return Token(TokenType::StringLiteral, currentLocation_, tokenBuffer_);
    }

    Token Lexer::getNextCharacterConstantToken() {
        char c;
        while(!fileWrapper_.eof() && (c = fileWrapper_.getNextChar()) != '\'') {
            tokenBuffer_.push_back(c);
        }
        return Token(TokenType::CharacterConstant, currentLocation_, tokenBuffer_);
    }

    bool isPermittedChar(const char c) {
        return c == '_' || isalnum(c);
    }

    Token Lexer::getNextIdentifierToken() {
        while(!fileWrapper_.eof() && isPermittedChar(fileWrapper_.peekChar(1))) {
            tokenBuffer_.push_back(fileWrapper_.getNextChar());
        }
#define KEYWORD(X, Y) if(tokenBuffer_ == Y) GOT_KEYWORD(KeyWord::X);
#include <def/Keyword.def>
        return Token(TokenType::Identifier, currentLocation_, tokenBuffer_);
    }
}
