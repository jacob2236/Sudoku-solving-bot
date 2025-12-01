#include "class_sudoku.cpp"
#pragma once



class RepresentedBoard{
    public:
        SudokuBoard base;
        vector<array<int, 3>> givens;
        RepresentedBoard(SudokuBoard newBoard) : base(newBoard), givens(newBoard.givens)
        { 
            
        }
        RepresentedBoard(){
        };

        // Called when creating a mutated sample of the board. 
        // returns NULL POINTER to reject the mutation if it unfixably breaks the board or representation
        // returns the board if no changes are needed or mild corrections could be completed
        bool validate(SudokuBoard newBoard){
            // check if givens are valid first
            for(int i = 0; i < givens.size(); i++) {
                array<int, 3> given = givens[i];
                if(newBoard.board[given[0]][given[1]] != given[2]) {
                    return false;
                }
            }
            // check for other deal-breakers, if they exist return NULL as well
            
            // check for minor changes that can be fixed

            // Construct a new RepresentedBoard based on the mutated and fixed up input
            return true;
        }

        RepresentedBoard newRep(SudokuBoard board) {
            return RepresentedBoard(board);
        }

};