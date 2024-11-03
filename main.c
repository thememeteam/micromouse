#include "world/world.h"
#include "solver/solver.h"

#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
    worldInit();

    Solver solver;
    solver.currentJunction = makeJunction();
    
    while (!done && getchar()) {
        Direction nextDir = nextDirection(solver.currentJunction);
        markDirection(solver.currentJunction, nextDir);
        moveToJunction(nextDir);
        
        Junction* newJunction = makeJunction();
        newJunction->prev = solver.currentJunction;
        solver.currentJunction->next = newJunction;
        
        markDirection(newJunction, DOWN);
        
        solver.currentJunction = newJunction;

        printMaze();
    }

    return 0;
}