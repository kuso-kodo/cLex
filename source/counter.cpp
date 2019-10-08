#include <cLex/counter.h>
#include <cLex/token.h>

namespace cLex {
    void Counter::update(const Token &token) {
        switch (token.getTokenType()) {
#define TOKEN_TYPE(X,Y) case TokenType::X: count_##X ++; return;
#include <def/TokenType.def>
#undef TOKEN_TYPE
            default:
                return;
        }
    }

    std::ostream& operator<<(std::ostream& os, const Counter & counter) {
#define TOKEN_TYPE(X,Y) os << Y  << ":\t" << counter.count_##X << std::endl;
#include <def/TokenType.def>
#undef TOKEN_TYPE
        return os;
    }
}