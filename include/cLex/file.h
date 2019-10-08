#ifndef CLEX_FILE_H
#define CLEX_FILE_H

#include <deque>
#include <fstream>
#include <utility>
#include <cassert>
#include "token.h"

namespace cLex {
    class FileWrapper {
    public:
        explicit FileWrapper(std::string fileName) : fileName_{std::move(fileName)}, line_{1}, column_{0}, eof_{false}, count_{0} {
            sourceFile_.open(fileName_);
            assert(!sourceFile_.fail());
            this->read();
        }

        char getNextChar() {
            if(this->eof())
                return '\0';

            if(buffer_.empty()) {
                this->read();
            }
            auto c = buffer_.front();
            buffer_.pop_front();

            // Update line_ and column_
            if(c == '\n') {
                line_++;
                column_ = 0;
            } else {
                column_++;
            }
            count_++;

            return c;
        }

        char peekChar(size_t const offset) {
            while(!sourceFile_.eof() && buffer_.size() < offset) {
                this->read();
            }
            if(buffer_.size() < offset) {
                throw std::out_of_range("Early EOF. Offset out of range.");
            }
            return buffer_.at(offset - 1);
        }

        void eatChars(size_t const num) {
            for(size_t i = 0; i < num; i++)
                getNextChar();
        }

        bool eof() {
            return eof_ && buffer_.empty();
        }

        [[nodiscard]] size_t getCount() const {
            return count_;
        }

        [[nodiscard]] size_t getLineCount() const {
            return line_;
        }

        [[nodiscard]] std::string getName() const {
            return fileName_;
        }

        friend std::ostream& operator<<(std::ostream& os, const FileWrapper & fileWrapper);
        TokenLocation getLocation();
    private:
        void read() {
            if(!eof_) {
                auto c = sourceFile_.get();
                buffer_.push_back(c);
                sourceFile_.peek();
                eof_ = sourceFile_.eof();
            }
        }

        std::string fileName_;
        std::ifstream sourceFile_;
        std::deque<char> buffer_;
        bool eof_;
        size_t line_;
        size_t column_;
        size_t count_;
    };
}

#endif //CLEX_FILE_H
