#pragma once

#include <stdbool.h>

#define MAZE_SIZE 33

typedef enum
{
    LEFT = 1,
    UP = 2,
    RIGHT = 4,
    DOWN = 8,
} DirectionValue;

typedef unsigned char Direction;

bool done = 0;

Direction oppositeSide(Direction direction)
{
    // rotate right by 2
    return ( direction >> 2 ) | ( ( direction << 2 ) & 15 );
}

Direction rotateLeft(Direction direction)
{
    return ((direction << 1) & 0b1111) | ((direction & 0b1000) >> 3);
}
