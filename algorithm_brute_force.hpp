#include "class_abstract_board.hpp"
#include "class_sudoku.hpp"
#ifndef ALGORITHM_BRUTE_FORCE 
#define ALGORITHM_BRUTE_FORCE

SudokuBoard oldBruteforce(SudokuBoard board);
AbstractBoard randomBruteForce(AbstractBoard inputBoard, int maxIter);
#endif