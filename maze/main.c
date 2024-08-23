#include "globals.h"
#include "defs.h"
#include "util.h"

#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void moveToJunction(char direction)
{
    char opposite = oppositeSide(direction);

    while (((validSides() & direction) != 0))
    {
        char end = canSeeEnd();
        if (end)
        {
            if (end & LEFT)
            {
                curY--;
            }
            else if (end & UP)
            {
                curX--;
            }
            else if (end & RIGHT)
            {
                curY++;
            }
            else if (end & DOWN)
            {
                curX++;
            }
            done = 1;
            break;
        }

        maze[curX][curY] += 1;
        if (direction & LEFT)
        {
            curY--;
        }
        else if (direction & UP)
        {
            curX--;
        }
        else if (direction & RIGHT)
        {
            curY++;
        }
        else if (direction & DOWN)
        {
            curX++;
        }

        if ((validSides() & (~(opposite | direction) & 0xf)) != 0)
            break;
    }
    maze[curX][curY] = 0;
}

int main()
{
    printf("\x1b[2J");

    readMaze();
    printMaze();

    curX = startX;
    curY = startY;

    char first = 1;

    while (!done)
    {
        char sides = validSides();

        // handle dead end
        if (!first && __builtin_popcount(sides) == 1)
        {
            moveToJunction(sides);
        }
        else
        {
            if (!first && __builtin_popcount(markedSides(sides)) == 1)
            {
                sides ^= markedSides(sides);
            }

            if (sides & LEFT)
            {
                moveToJunction(LEFT);
            }
            else if (sides & UP)
            {
                moveToJunction(UP);
            }
            else if (sides & RIGHT)
            {
                moveToJunction(RIGHT);
            }
            else if (sides & DOWN)
            {
                moveToJunction(DOWN);
            }
            first = 0;
        }

        printMaze();
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 1e+8;
        nanosleep(&ts, NULL);
    }

    return 0;
}