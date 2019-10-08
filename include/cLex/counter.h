#ifndef CLEX_COUNTER_H
#define CLEX_COUNTER_H

#include <cLex/token.h>
namespace cLex {
    class Counter {
    public:
        void update(const Token& token);
        friend std::ostream& operator<<(std::ostream& os, const Counter & counter);
    private:
#define TOKEN_TYPE(X, Y) size_t count_##X;
#include <def/TokenType.def>
#undef TOKEN_TYPE
    };
}

#endif //CLEX_COUNTER_H
