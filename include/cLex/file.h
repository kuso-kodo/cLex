#ifndef CLEX_FILE_H
#define CLEX_FILE_H

#include <deque>
#include <fstream>

class FileWrapper {
public:
    FileWrapper(std::ifstream& file) : sourceFile_{file} {
        this->read();
    }

    char getNextChar() {
        auto c = buffer_.front();
        buffer_.pop_front();
        if(buffer_.empty()) {
            this->read();
        }
    }

    char peekChar(size_t const offset) {
        while(sourceFile_ && buffer_.size() < offset) {
            this->read();
        }
        if(buffer_.size() < offset) {
            throw std::out_of_range("Early EOF. Offset out of range.");
        }
        return buffer_.at(offset);
    }
private:
    void read() {
        buffer_.emplace_back();
        if(!(sourceFile_ >> buffer_.back())) {
            buffer_.pop_back();
        }
    }
    std::ifstream& sourceFile_;
    std::deque<char> buffer_;
};

#endif //CLEX_FILE_H
