#include "class_represented_board.hpp"
#include <set>

#ifndef REPRESENTED_ROWCONSISTENT 
#define REPRESENTED_ROWCONSISTENT

class RepresentedRowConsistent : public RepresentedBoard {
    // This representation maintains that each row always exactly only contains each digit once. 
    // Conflicts in columns and groups are permitted, but never rows. Zeros are not permitted
    // This concept comes from paper [9]
    public:
        SudokuBoard base;
        vector<array<int, 3>> givens;
        RepresentedRowConsistent(SudokuBoard newBoard);
        RepresentedRowConsistent();
        bool validate(SudokuBoard &newBoard);
        RepresentedRowConsistent newRep(SudokuBoard board);

};

#endif