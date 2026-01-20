#include "class_sudoku.hpp"
#include <set>
#include <functional>

#ifndef FITNESS_FUNCTIONS 
#define FITNESS_FUNCTIONS 

int countConflicts(SudokuBoard currentBoard);
int countPossibilities(SudokuBoard currentBoard);


#endif