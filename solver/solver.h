#include "../common.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Junction {
    struct Junction* next;
    struct Junction* prev;
    int marks[4]; 
} Junction;

typedef struct {
    Junction* currentJunction;
} Solver;

int directionToIndex(Direction dir) {
    switch (dir) {
        case LEFT: return 0;
        case UP: return 1;
        case RIGHT: return 2;
        case DOWN: return 3;
        default: return -1;
    }
}

Direction indexToDirection(int index) {
    switch (index) {
        case 0: return LEFT;
        case 1: return UP;
        case 2: return RIGHT;
        case 3: return DOWN;
        default: return 0;
    }
}

Junction* makeJunction() {
    Junction* junction = (Junction*)malloc(sizeof(Junction));
    if (junction) {
        junction->next = NULL;
        junction->prev = NULL;
        for (int i = 0; i < 4; i++) {
            junction->marks[i] = 0;
        }
    }
    return junction;
}

Direction nextDirection(Junction* junction) {
    int availablePaths = validSides();
    Direction leastMarkedDir = 0;
    int leastMarks = junction->marks[0];
    
    for (int i = 0; i < 4; i++) {
        Direction currentDir = indexToDirection(i);
        if (!(availablePaths & currentDir)) {
            continue;
        }
        
        if (junction->marks[i] == 0) {
            return junction->marks[i];
        }
        
        if (junction->marks[i] < leastMarks) {
            leastMarks = junction->marks[i];
            leastMarkedDir = currentDir;
        }
    }
    
    return leastMarkedDir;
}

void markDirection(Junction* junction, Direction dir) {
    int index = directionToIndex(dir);
    if (index >= 0) {
        junction->marks[index]++;
    }
}