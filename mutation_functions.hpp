#include "class_sudoku.hpp"

#ifndef MUTATION_FUNCTIONS 
#define MUTATION_FUNCTIONS

SudokuBoard swapInRow(SudokuBoard newBoard);
SudokuBoard fillOrRemove(SudokuBoard newBoard);
SudokuBoard fillPossibilityOrRemove(SudokuBoard newBoard);
bool isLegalSwap(const SudokuBoard &board, int row, int col1, int col2);
SudokuBoard rowSwapBase(SudokuBoard newBoard, bool check);
SudokuBoard rowSwap(SudokuBoard newBoard);
SudokuBoard rowSwapCheck(SudokuBoard newBoard);

#endif