#include "class_sudoku.hpp"

#ifndef REPRESENTED_BOARD 
#define REPRESENTED_BOARD

class RepresentedBoard{
    public:
        SudokuBoard base;
        vector<array<int, 3>> givens;
        RepresentedBoard(SudokuBoard newBoard);
        RepresentedBoard();
        virtual bool validate(SudokuBoard &newBoard);
        RepresentedBoard newRep(SudokuBoard board);
};

#endif