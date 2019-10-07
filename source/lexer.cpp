#include <cLex/lexer.h>

#define GOT_TOKEN(TYPE, VALUE) return Token(TYPE, currentLocation_, VALUE)
#define GOT_PUNCTOATOR(X) GOT_TOKEN(TokenType::Punctuator, X)

namespace cLex {
    Token Lexer::getNextToken() {
        tokenBuffer_.clear();
        if(fileWrapper_.eof())
            return Token(TokenType::EndOfFile, fileWrapper_.getLocation(), "");
        auto c = fileWrapper_.getNextChar();
        while(isblank(c) || c == '\n') {
            c = fileWrapper_.getNextChar();
            if(fileWrapper_.eof())
                return Token(TokenType::EndOfFile, fileWrapper_.getLocation(), "");
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
                }
            default:
                tokenBuffer_.push_back(c);
                break;
        }
        auto result = Token(TokenType::Unknown, fileWrapper_.getLocation(), tokenBuffer_);
        return result;
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
                (c == 'x') || (c == 'X') || (c == 'p') || (c == 'l') || (c == 'L');
    }

    Token Lexer::getNextNumericToken() {

        return Token(TokenType::Unknown, fileWrapper_.getLocation(), tokenBuffer_);
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
}