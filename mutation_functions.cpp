#include "class_sudoku.cpp"
#include <set>
/* This should contain all mutation functions for easy include accessing
An abstract mutator function exists as a blueprint/pseudocode 
Mutation functions MUST operate only on a basic SudokuBoard to be agnostic to representations
*/


SudokuBoard swapInRow(SudokuBoard newBoard) {
    //mutates by swapping two digits within a row
    int row = rand() % 9;
    int digit1 = rand() % 9;
    int digit2;
    do {
        digit2 = rand() % 9;
    } while(digit2 == digit1); // reroll until they're two different digits
    int temp = newBoard.board[row][digit1];
    newBoard.board[row][digit1] = newBoard.board[row][digit2];
    newBoard.board[row][digit2] = temp;
    return newBoard;
}

SudokuBoard fillOrRemove(SudokuBoard newBoard){
    int randRow = rand() % 9;
    int randCol = rand() % 9;
    if(newBoard.board[randRow][randCol] == 0) {
        newBoard.board[randRow][randCol] = rand() % 9;
    } else {
        newBoard.board[randRow][randCol] = 0;
    }
    return newBoard;
}

SudokuBoard fillPossibilityOrRemove(SudokuBoard newBoard){
    // like fillOrRemove() but it considers the row/col/group consistency of a cell
    int randRow = rand() % 9;
    int randCol = rand() % 9;
    if(newBoard.board[randRow][randCol] == 0) {
        // find possible digits
        set<int> totalSet = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for(int i = 0; i < 0; i++) { // remove digits present in the row/col
            totalSet.erase(newBoard.board[randRow][i]);
            totalSet.erase(newBoard.board[i][randCol]);
        }
        //remove digits present in the group
        auto erasePossibilities = [&totalSet](int group[9]) {
            for(int i = 0; i < 9; i++) {
                totalSet.erase(group[i]);
            }
        };
        newBoard.forThisGroup(erasePossibilities, randRow, randCol);
        if (totalSet.empty()) {
            // If the chosen cell has no possibilities, try again with a new random cell.
            // Should not loop too many times since having an empty cell with no possibilities 
            // necessarily implies several filled cells that can be unfilled instead to end this loop
            return fillPossibilityOrRemove(newBoard);
        } else {
            int choice = rand() % totalSet.size();
            int count = 0;
            for(int digit : totalSet) {
                if (count == choice) {
                    newBoard.board[randRow][randCol] = digit;
                    return newBoard;
                }
                count++;
            }
        }
    } else {
        newBoard.board[randRow][randCol] = 0;
    }
    return newBoard;

}