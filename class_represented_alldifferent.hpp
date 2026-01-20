#include "class_represented_board.hpp"
#include <set>

#ifndef REPRESENTED_ALLDIFFERENT
#define REPRESENTED_ALLDIFFERENT

class RepresentedAllDifferent: public RepresentedBoard {
    public:
        SudokuBoard base;
        vector<array<int, 3>> givens;
        RepresentedAllDifferent(SudokuBoard newBoard);
        RepresentedAllDifferent();
        bool validate(SudokuBoard &newBoard);
        RepresentedAllDifferent newRep(SudokuBoard board);
};

#endif