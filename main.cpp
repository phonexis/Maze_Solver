//
//  main.cpp
//  Maze
//
//  Created by Dorjee Gyaltsen on 11/5/18.
//  Copyright © 2018 Dorjee Gyaltsen. All rights reserved.
//

#include "MazeSolver.h"
int main() {
    MazeSolver solver("input.txt");
    if(solver.mazeIsReady())
    {
        solver.solveMaze();
        solver.printSolution();
    }
}
return 0;

