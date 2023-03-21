#include "Buffer.h"

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::append(const char *str, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (str[i] == '\0')
            break;
        buf.push_back(str[i]);
    }
}

ssize_t Buffer::size() const
{
    return buf.size();
}

const char *Buffer::c_str() const
{
    return buf.c_str();
}

void Buffer::clear()
{
    buf.clear();
}
