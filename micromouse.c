#include <common.h>

#include <pico/stdlib.h>
#include <hardware/pwm.h>

#include <world/world-pico.h>
#include <solver/solver.h>

#include <string.h>
#include <stdio.h>

int main()
{
    stdio_init_all();

    worldInit();
    
    bool enabled = true; 

    Solver solver;
    solver.currentJunction = makeJunction();
    
    while (!done && getchar())
    {
        Direction nextDir = nextDirection(solver.currentJunction);
        markDirection(solver.currentJunction, nextDir);
        moveToJunction(nextDir);
        
        Junction* newJunction = makeJunction();
        newJunction->prev = solver.currentJunction;
        solver.currentJunction->next = newJunction;
        
        markDirection(newJunction, DOWN);
        
        solver.currentJunction = newJunction;
    }

    for(;;) { }
}
