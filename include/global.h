#pragma once

#include "gba/gba.h"

#define packed __attribute__((packed))
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
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
