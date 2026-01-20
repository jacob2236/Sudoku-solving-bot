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
// returns NULL POINTER to reject the mutation if it unfixably breaks the board or representation
// returns the board if no changes are needed or mild corrections could be completed
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
        set<int> missingDigits ={};
        for (int i = 0; i < 9; i++) {
            if (!foundDigits.count(i)) {
                int col = *invalids.begin();
                newBoard.board[row][col] = i;
                invalids.erase(col);
            }
        }

    }
    return true;
}

RepresentedRowConsistent RepresentedRowConsistent::newRep(SudokuBoard board) {
    return RepresentedRowConsistent(board);
}