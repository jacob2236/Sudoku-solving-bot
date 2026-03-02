#include "class_represented_rowconsistent.hpp"


// This representation maintains that each row always exactly only contains each digit once. 
// Conflicts in columns and groups are permitted, but never rows. Zeros are not permitted
// This concept comes from paper [9]

RepresentedRowConsistent::RepresentedRowConsistent(SudokuBoard newBoard) : base(newBoard), givens(newBoard.givens)
{ 
    
}
RepresentedRowConsistent::RepresentedRowConsistent(){
};

// Called when creating a mutated sample of the board. 
// returns false to reject the mutation
// modifies the board if its in error.
bool RepresentedRowConsistent::validate(SudokuBoard &newBoard){

    // check if givens are valid first
    for(int i = 0; i < givens.size(); i++) {
        array<int, 3> given = givens[i];
        if(newBoard.board[given[0]][given[1]] != given[2]) {
            return false; //invalid and irreconcilable board
        }
    }

    for(int row = 0; row < 9; row++) {
        set<int> foundDigits = {};
        set<int> invalids = {};
        for (int col = 0; col < 9; col++) {
            int digit = newBoard.board[row][col];
            if (digit == 0 || foundDigits.count(digit)) {
                // Duplicate or zero. Neither is tolerated
                invalids.insert(col);
            } else {
                foundDigits.insert(digit);
            }
        }

        //replace invalid digits with valid ones. 
        for (int i = 0; i < 9; i++) {// for each digit
            if (!foundDigits.count(i)) { // if it's not found in the row
                int col = *invalids.begin(); // find the first cell in the row with a bad digit
                newBoard.board[row][col] = i; // put the digit in that spot
                invalids.erase(col); // take the cell out of the set of invalid cells
            }
        }

    }
    return true;
}

RepresentedRowConsistent RepresentedRowConsistent::newRep(SudokuBoard board) {
    return RepresentedRowConsistent(board);
}