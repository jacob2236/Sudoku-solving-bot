#include "class_sudoku.cpp"
/* This is a GENERIC DEFAULT class from which other classes with actual representations must inherit
    This should not be used directly and any code in them is purely example code for the expectations 
    And responsibilities of the subclasses.
*/


class RepresentedBoard{
    public:
        SudokuBoard base;
        vector<array<int, 3>> givens;
        RepresentedBoard(SudokuBoard newBoard = SudokuBoard()){
            base = newBoard;
            givens = newBoard.givens;
        }

        // Called when creating a mutated sample of the board. 
        // returns NULL POINTER to reject the mutation if it unfixably breaks the board or representation
        // returns the board if no changes are needed or mild corrections could be completed
        RepresentedBoard* validate(SudokuBoard newBoard){
            // check if givens are valid first
            for(int i = 0; i < givens.size(); i++) {
                array<int, 3> given = givens[i];
                if(newBoard.board[given[0]][given[1]] != given[2]) {
                    return NULL; //invalid and irreconcilable board
                }
            }
            // check for other deal-breakers, if they exist return NULL as well

            // check for minor changes that can be fixed

            // Construct a new RepresentedBoard based on the mutated and fixed up input
            RepresentedBoard result = RepresentedBoard(newBoard);
            RepresentedBoard* resultPointer = &result;
            return resultPointer;
        }

};