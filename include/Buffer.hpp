#ifndef __BUFFER_HPP
#define __BUFFER_HPP

#include <sstream>
#include <fstream>

class Buffer {
public:
    Buffer(int bufferNum = 1024);
    Buffer(const std::string& str);

    std::string getLine();
    std::string getAll();

    Buffer& operator<<(const std::fstream& fs);
    Buffer& operator<<(Buffer& buffer);

    Buffer& operator<<(const std::string& str);
    Buffer& operator<<(int i);
    Buffer& operator<<(double d);
    Buffer& operator<<(long int l);

    void clear();
    int count();

private:
    int count_;
    std::stringstream bufferStream_;
    int maxSize_;
};

#endif //__BUFFER_HPP
