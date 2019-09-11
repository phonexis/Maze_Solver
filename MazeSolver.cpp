//
//  MazeSolver.cpp
//  Maze
//  CSCI 235
//  Created by Dorjee Gyaltsen on 11/5/18.
//  Copyright © 2018 Dorjee Gyaltsen. All rights reserved.
//

#ifndef MAZESOLVER_CPP
#define MAZESOLVER_CPP

#include <fstream>
#include <iostream>
#include <array>
#include <stack>
#include "MazeSolver.h"
#include <stdio.h>
#include <string>

using namespace std;

//constructor
//pre: input file is in correct format with first two values being integers
//      followed by valid maze characters in {'_', '*','$'}
//post: if inuput file cannot be read outputs "Cannot read from input_file"
//      otherwise sets maze_rows_ and maze_columns_ from the first two values in input file
//      and allocates two 2-dimesional array of size maze_rows_ and maze_columns_
//      both maze_ and solution_ are initialized with characters read from input
MazeSolver::MazeSolver(std::string input_file){
    ifstream input;
    input.open(input_file);
    if(input.fail())
        cout << "Cannot read from input_file"<<endl;
    else{
        input>>maze_rows_>>maze_columns_;
        initializeMaze(maze_rows_, maze_columns_);
        fillMaze(input);
        initializeSolution();
        copyMazetoSolution();
        maze_ready = true;
    }
}

//post: deletes maze_ and solution_
MazeSolver::~MazeSolver(){
    delete maze_;
    delete solution_;
}

//return: true if maze has been initialized, false otherwise
bool MazeSolver::mazeIsReady(){
    return maze_ready;
}

//pre: maze_ has been initialized with valid character values in {'_', '*','$'}
//post: solution_ has been marked with '>' signs along the path to the exit
//      prints "Found the exit!!!" if exit is found
//      or "This maze has no solution." if no exit could be found
//return: true if exit is found, false otherwise
bool MazeSolver::solveMaze(){
    Position current_position = {0,0};
    backtrack_stack_.push(current_position);
    
    while(!backtrack_stack_.empty()){
        current_position = backtrack_stack_.top();
        int currentRow = current_position.row;
        int currentCol = current_position.column;
        if(solution_[currentRow][currentCol] == '$'){
            cout<<"Found the exit!!!"<<endl;
            return true;
        }
        else if(extendPath(current_position)){
            solution_[currentRow][currentCol] = '>';
            current_position = backtrack_stack_.top();
        }
        
        else{
            maze_[currentRow][currentCol] = 'X';
            solution_[currentRow][currentCol]= '@';
            backtrack_stack_.pop();
            if(!backtrack_stack_.empty()){
                current_position = backtrack_stack_.top();
            }
            else{
                cout <<"This maze has no solution."<<endl;
                return false;
            }
        }
    }
    return false;
}


//post: prints the solution to the standard output stream
//      with single space character between each maze character
//      and each maze row on a new line
void MazeSolver::printSolution(){
    cout<<"The solution to this maze is: ";
    for(int i=0; i<maze_rows_; i++){
        cout<<endl;
        for(int j=0; j<maze_columns_;j++){
            cout<< solution_[i][j];
            if(j != maze_columns_ -1){
                cout<<" ";
            }
        }
    }
    cout<<endl;
}


//pre: rows and columns are positive integers
//post: allocates maze_ with rows and columns
//called by constructor
void MazeSolver::initializeMaze(int rows, int columns){
    maze_ = new char*[rows];
    for (int i=0; i<rows; i++){
        maze_[i] = new char[columns];
    }
    
}


//pre: maze_ has been allocated with the correct number of rows and columns read from input file
//post: fills in maze_ with characters read from input file
//called by constructor
void MazeSolver::fillMaze(std::ifstream& input_stream){
    input_stream.seekg(0);
    string line;
    getline(input_stream, line);
    int occur1 = line.find(" ");
    int occur2 = line.find(" ", occur1+1);
    int start = occur2+1;
    
    for(int i=0; i<maze_rows_; i++)
    {
        for(int j=0; j<maze_columns_; j++){
            int index = (start)+(i*2*maze_columns_+j*2);
            maze_[i][j] = line[index];
        }
    }
}

//pre: maze_ has been initialized with valid character values in {'_', '*','$'}
//     start position is always [0][0]
//post: initializes solution_ with a copy of maze_
//      initializes backtrack_stack_  with all viable paths from position [0][0]
//      and mark the current position as visited ( '>' ) on solution_
//called by constructor
void MazeSolver::initializeSolution(){
    solution_=new char*[maze_rows_];
    for(int i=0; i<maze_rows_; i++){
        solution_[i]=new char[maze_columns_];
    }
    //cout<<endl;
}


//pre: maze_ has been properly initialized
//post: allocates solution_ to the correct number of rows and columns
//      and copies the contents of maze_ into solution_
//called by initializeSolution()
void  MazeSolver::copyMazetoSolution(){
    // cout<<endl<<"This is solution_:"<<endl;
    for(int i=0; i<maze_rows_; i++){
        for(int j=0; j<maze_columns_; j++){
            solution_[i][j] = maze_[i][j];
        }
    }
    //cout<<endl;
} 

//pre: current_position is a valid position on the maze_
//post: adds all positions extensible from current_position to backtrack_stack_
//return: true if path was extended, false otherwise
//called by solveMaze()
bool MazeSolver::extendPath(Position current_position){
    bool extended = false;
    if(isExtensible(current_position, SOUTH)){
        backtrack_stack_.push(getNewPosition(current_position,SOUTH));
        extended = true;
    }
    if(isExtensible(current_position, EAST)){
        backtrack_stack_.push(getNewPosition(current_position, EAST));
        extended = true;
    }
    return extended;
}


//pre: old_position is a Position initialized with row and column to valid positions in maze_ and it is extensible in direction dir
//return: a new Position on the maze moving in direction dir from old_position
//called by extendPath()
Position MazeSolver::getNewPosition(Position old_position, direction dir){
    int oldRow = old_position.row;
    int oldCol = old_position.column;
    
    if(dir == SOUTH){
        Position new_position = {oldRow+1, oldCol};
        
        return new_position;
    }
    else if(dir == EAST){
        Position new_position = {oldRow, oldCol+1};
        
        return new_position;
    }
    else{
        cout<<"Error! Enter a valid direction."<<endl;
    }
    return old_position;
}


//checks if the path can be extended in maze_ from position current_position in direction dir
//return: true if path can be extended given current_position and dir, false otherwise
//called by extendPath
bool MazeSolver::isExtensible(Position current_position, direction dir){
    int currentRow = current_position.row;
    int currentCol = current_position.column;
    bool extensible = false;
    
    if(dir == SOUTH){
        int newRow = currentRow+1;
        int newCol = currentCol;
        
        if(currentRow < maze_rows_-1){
            if(solution_[newRow][newCol] == '_' || solution_[newRow][newCol] == '$'){
                extensible = true;
            }
        } else{
            return false;
        }
    } else if(dir == EAST){
        int newRow = currentRow;
        int newCol = currentCol+1;
        
        if(currentCol < maze_columns_-1){
            if(solution_[newRow][newCol]=='_' || solution_[newRow][newCol]=='$'){
                extensible = true;
            }
        }else{
            return false;
        }
    }else{
        cout<<"Error! Enter a valid direction."<<endl;
    }
    return extensible;
}

#endif



