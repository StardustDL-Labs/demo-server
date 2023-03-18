#include <cstdio>
#include <cstdlib>

#include "util.h"

void ensure(bool condition, const char *message)
{
    if (!condition)
    {
        perror(message);
        exit(EXIT_FAILURE);
    }
}