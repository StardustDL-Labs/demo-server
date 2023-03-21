#pragma once
#include <vector>
#include <string>
#include <cstdlib>

class Buffer
{
private:
    std::string buf;

public:
    Buffer();
    ~Buffer();
    void append(const char *str, int size);
    ssize_t size() const;
    const char *c_str() const;
    void clear();
};