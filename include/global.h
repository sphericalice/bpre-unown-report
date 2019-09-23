#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "gba/gba.h"
#include "constants/global.h"

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define T1_READ_32(ptr) ((ptr)[0] | ((ptr)[1] << 8) | ((ptr)[2] << 16) | ((ptr)[3] << 24))
#define T1_READ_PTR(ptr) (u8*) T1_READ_32(ptr)

struct Coords16
{
    s16 x;
    s16 y;
};

#endif // GUARD_GLOBAL_H