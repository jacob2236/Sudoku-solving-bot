#include "class_represented_board.cpp"
#include <set>

class RepresentedAllDifferent: public RepresentedBoard {
    // This representation expects all filled cells to maintain Row, Column, and Group consistency
    // It will consider any board that does not maintain these consistencies invalid
    // zeros can be wildcards and are not considered for consistency
    public:
        SudokuBoard base;
        vector<array<int, 3>> givens;
        RepresentedAllDifferent(SudokuBoard newBoard) : RepresentedBoard(newBoard)
        { 
            
        }
        RepresentedAllDifferent(){
        };

        // Called when creating a mutated sample of the board. 
        // returns NULL POINTER to reject the mutation if it unfixably breaks the board or representation
        // returns the board if no changes are needed or mild corrections could be completed
        RepresentedAllDifferent* validate(SudokuBoard newBoard){
            // check if givens are valid first
            for(int i = 0; i < givens.size(); i++) {
                array<int, 3> given = givens[i];
                if(newBoard.board[given[0]][given[1]] != given[2]) {
                    return NULL; //invalid and irreconcilable board
                }
            }
            // check for other deal-breakers, if they exist return NULL as well
            bool duplicates = false;
            auto getDuplicates = [&duplicates](int cells[9]) {
                set<int> foundDigits = {};
                for(int i = 0; i < 9; i++) {
                    int digit = cells[i];
                    foundDigits.insert(digit);
                }
                duplicates = false;
            };
            newBoard.forEachRow(getDuplicates);
            if(duplicates) {
                return NULL;
            } 

            newBoard.forEachCol(getDuplicates);
            if (duplicates) {
                return NULL;
            }
        
            newBoard.forEachGroup(getDuplicates); 
            if (duplicates) {
                return NULL;
            }
            
            
            // I'd love to have a minor adjustments portion here, but 
            // If there existed a quick/clean way to resolve duplicates
            // in less than like 2^N time, we wouldn't be making metaheuristics would we?


            // Construct a new RepresentedBoard based on the mutated and fixed up input
            RepresentedAllDifferent result = RepresentedAllDifferent(newBoard);
            RepresentedAllDifferent* resultPointer = &result;
            return resultPointer;
        }

};