#pragma once

#include "gba/gba.h"
#include "constants/global.h"
#define packed __attribute__((packed))
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
// There are many quirks in the source code which have overarching behavioral differences from
// a number of other files. For example, diploma.c seems to declare rodata before each use while
// other files declare out of order and must be at the beginning. There are also a number of
// macros which differ from one file to the next due to the method of obtaining the result, such
// as these below. Because of this, there is a theory (Two Team Theory) that states that these
// programming projects had more than 1 "programming team" which utilized different macros for
// each of the files that were worked on.
#define T1_READ_8(ptr)  ((ptr)[0])
#define T1_READ_16(ptr) ((ptr)[0] | ((ptr)[1] << 8))
#define T1_READ_32(ptr) ((ptr)[0] | ((ptr)[1] << 8) | ((ptr)[2] << 16) | ((ptr)[3] << 24))
#define T1_READ_PTR(ptr) (u8*) T1_READ_32(ptr)
struct Pokemon;

struct Coords16
{
    s16 x;
    s16 y;
};

struct UCoords16
{
    u16 x;
    u16 y;
};
