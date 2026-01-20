#include "class_sudoku.hpp"
#ifndef BOARD_LOADER_H
#define BOARD_LOADER_H

// total boards per data set
const int puzzle01MaxSize = 1000000;
const int puzzle2MaxSize = 49158;
const int puzzle5MaxSize = 48766;

SudokuBoard getSpecifiedBoard(int index);
SudokuBoard getRandomBoard();

SudokuBoard getSpecific17Board(int index);
SudokuBoard getRandom17Board();

SudokuBoard getSpecificHardBoard(int index);
SudokuBoard getRandomHardBoard();
#endif