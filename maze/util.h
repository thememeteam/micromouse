#pragma once

#include "defs.h"
#include <stdio.h>

uint8_t maze2[MAZE_SIZE][MAZE_SIZE];

// void readMaze()
// {
//     FILE *thing = fopen("maze.txt", "r");
//     if (!thing)
//         return;
//     for (int i = 0; i < MAZE_SIZE; i++)
//     {
//         for (int j = 0; j < MAZE_SIZE; j++)
//         {
//             char next = fgetc(thing);
//             if (next == 'S')
//             {
//                 startX = i;
//                 startY = j;
//                 maze[i][j] = 'S';
//             }
//             else if (next == 'E')
//             {
//                 endX = i;
//                 endY = j;
//                 maze[i][j] = 'E';
//             }
//             else
//             {
//                 maze[i][j] = next == '#' ? -1 : 0;
//             }
//         }
//         fgetc(thing);
//     }
// }

void readMaze()
{
    printf("Enter maze as string (temporary!): ");
    uart_read_blocking(uart0, &maze2, sizeof(maze2));
    memcpy((void *)maze, maze2, sizeof(maze2));
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            signed char next = maze[i][j];
            if (next == 'S')
            {
                startX = i;
                startY = j;
                maze[i][j] = 'S';
            }
            else if (next == 'E')
            {
                endX = i;
                endY = j;
                maze[i][j] = 'E';
            }
            else
            {
                maze[i][j] = next == '#' ? -1 : 0;
            }
        }
    }
    printf("\x1b[2J");
    printf("\x1b[00H");
}

void printMaze()
{
    printf("\x1b[00H");
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            printf("\x1B[0m");
            signed char next = maze[i][j];
            char *thing = "  ";
            if (i == curX && j == curY)
            {
                thing = "ii";
            }
            else
            {
                switch (next)
                {
                case 'S':
                    thing = "S ";
                    break;
                case 'E':
                    thing = "E ";
                    break;
                case -1:
                    thing = "||";
                    break;
                case 1:
                    thing = "..";
                    break;
                case 2:
                    thing = "xx";
                    break;
                };
            }
            printf("%s", thing);
        }
        printf("\n");
    }
    fflush(stdout);
}

char oppositeSide(char direction)
{
    char opposite = 0;
    switch (direction)
    {
    case LEFT:
        opposite = RIGHT;
        break;
    case UP:
        opposite = DOWN;
        break;
    case RIGHT:
        opposite = LEFT;
        break;
    case DOWN:
        opposite = UP;
        break;
    }

    return opposite;
}

// left top right bottom
char validSides()
{
    char res = 0;
    if (curY != 0 && maze[curX][curY - 1] != -1 && maze[curX][curY - 1] < 2)
        res |= LEFT;
    if (curX != 0 && maze[curX - 1][curY] != -1 && maze[curX - 1][curY] < 2)
        res |= UP;
    if (curY != MAZE_SIZE - 1 && maze[curX][curY + 1] != -1 && maze[curX][curY + 1] < 2)
        res |= RIGHT;
    if (curX != MAZE_SIZE - 1 && maze[curX + 1][curY] != -1 && maze[curX + 1][curY] < 2)
        res |= DOWN;

    return res;
}

char markedSides(char validSides)
{
    char marked = 0;
    if ((validSides & LEFT) && maze[curX][curY - 1] == 1)
        marked |= LEFT;
    if ((validSides & UP) && maze[curX - 1][curY] == 1)
        marked |= UP;
    if ((validSides & RIGHT) && maze[curX][curY + 1] == 1)
        marked |= RIGHT;
    if ((validSides & DOWN) && maze[curX + 1][curY] == 1)
        marked |= DOWN;

    return marked;
}

char canSeeEnd()
{
    char res = 0;
    if (maze[curX][curY - 1] == 'E')
    {
        res |= LEFT;
    }
    else if (maze[curX - 1][curY] == 'E')
    {
        res |= UP;
    }
    else if (maze[curX][curY + 1] == 'E')
    {
        res |= RIGHT;
    }
    else if (maze[curX + 1][curY] == 'E')
    {
        res |= DOWN;
    }
    return res;
}