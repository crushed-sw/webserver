#include "../include/Buffer.hpp"

Buffer::Buffer(int numBuffer) : maxSize_(numBuffer), count_(0) {}

Buffer::Buffer(const std::string& str) : Buffer(1024) {
    bufferStream_ << str;
}

std::string Buffer::getLine() {
    std::string str = "";
    getline(bufferStream_, str);
    count_--;
    return std::move(str);
}

std::string Buffer::getAll() {
    std::string str = bufferStream_.str();
    count_ = 0;
    bufferStream_.clear();
    return std::move(str);
}

Buffer& Buffer::operator<<(const std::fstream& fs) {
    if(!fs.is_open())
        throw std::runtime_error("the file stream is not open");

    bufferStream_ << fs.rdbuf();
    return *this;
}

Buffer& Buffer::operator<<(Buffer& buffer) {
    bufferStream_ << buffer.getAll();
    count_ += buffer.count_;
    return *this;
}

Buffer& Buffer::operator<<(const std::string& str) {
    bufferStream_ << str;
    count_++;
    return *this;
}

Buffer& Buffer::operator<<(int i) {
    bufferStream_ << i;
    count_++;
    return *this;
}

Buffer& Buffer::operator<<(double d) {
    bufferStream_ << d;
    count_++;
    return *this;
}

Buffer& Buffer::operator<<(long int l) {
    bufferStream_ << l;
    count_++;
    return *this;
}

void Buffer::clear() {
    bufferStream_.str("");
    bufferStream_.clear();
}

int Buffer::count() {
    return count_;
}
