#include <cLex/file.h>

namespace cLex {
    std::ostream& operator<<(std::ostream& os, const FileWrapper & fileWrapper) {
        os << fileWrapper.fileName_ << ":" << fileWrapper.line_ << ":" << fileWrapper.column_;
        return os;
    }

    TokenLocation FileWrapper::getLocation() {
        return TokenLocation(fileName_, column_, line_);
    }
}