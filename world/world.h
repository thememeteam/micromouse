#pragma once

#include "../common.h"
#include <stdio.h>
// #include "pico/stdlib.h"

signed char maze[MAZE_SIZE][MAZE_SIZE];

int startX;
int startY;

int endX;
int endY;

int curX;
int curY;

char done = 0;

char currentDirection = LEFT;

char adjust(char val, char targetDir, char currentDir)
{
    while (currentDir != targetDir)
    {
        currentDir = rotateLeft(currentDir);
        val = rotateLeft(val);
    }
    return val;
}

void readMaze()
{
    FILE *thing = fopen("maze.txt", "r");
    if (!thing)
        return;
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            char next = fgetc(thing);
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
        fgetc(thing);
    }
    fclose(thing);
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

void worldInit()
{
    printf("\x1b[2J");

    readMaze();
    printMaze();

    curX = startX;
    curY = startY;

    char first = 1;
}

Direction validSides()
{
    Direction res = 0;
    if (curY != 0 && maze[curX][curY - 1] != -1 && maze[curX][curY - 1] < 2)
        res |= LEFT;
    if (curX != 0 && maze[curX - 1][curY] != -1 && maze[curX - 1][curY] < 2)
        res |= UP;
    if (curY != MAZE_SIZE - 1 && maze[curX][curY + 1] != -1 && maze[curX][curY + 1] < 2)
        res |= RIGHT;
    if (curX != MAZE_SIZE - 1 && maze[curX + 1][curY] != -1 && maze[curX + 1][curY] < 2)
        res |= DOWN;

    return adjust(res, UP, currentDirection);
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

void moveToJunction(char direction)
{
    char opposite = oppositeSide(direction);
    char direction2 = adjust(direction, currentDirection, UP);

    while ((validSides() & direction) != 0)
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
        if (direction2 & LEFT)
        {
            curY--;
        }
        else if (direction2 & UP)
        {
            curX--;
        }
        else if (direction2 & RIGHT)
        {
            curY++;
        }
        else if (direction2 & DOWN)
        {
            curX++;
        }

        if ((validSides() & (~(opposite | direction) & 0xf)) != 0)
            break;
    }
    maze[curX][curY] = 0;
    currentDirection = direction2;
}