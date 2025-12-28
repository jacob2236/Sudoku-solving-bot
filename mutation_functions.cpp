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

// For the record, I think this shouldn't be part of the mutation function (and I will make/test a variant w/out it)
// The point of mutating IS to be random and kinda wrong. If every move is right, we just end up stuck in local optima
bool isLegalSwap(const SudokuBoard &board, int row, int col1, int col2) {
    int digit1 = board.board[row][col1];
    int digit2 = board.board[row][col2];

    // check column c1 for digit2 excluding position (row,c2)
    for (int r = 0; r < 9; ++r) {
        if (r == row) continue;
        if (board.board[r][col1] == digit2) return false;
    }
    // check column c2 for digit1 excluding (row,c1)
    for (int r = 0; r < 9; ++r) {
        if (r == row) continue;
        if (board.board[r][col2] == digit1) return false;
    }

    // check block for new positions
    auto blockTopLeft = [](int r, int c){ return std::make_pair((r/3)*3,(c/3)*3); };

    //for target (row,c1) with value digit2 
    auto topLeft = blockTopLeft(row, col1);
    for (int rr = topLeft.first; rr < topLeft.first+3; ++rr) {
        for (int cc = topLeft.second; cc < topLeft.second+3; ++cc) {
            if (rr == row && cc == col2) continue; // original location of digit2
            if (rr == row && cc == col1) continue; // skip itself
            if (board.board[rr][cc] == digit2) return false;
        }
    }

    // for target (row,c2) with value digit1
    topLeft = blockTopLeft(row, col2);
    for (int rr = topLeft.first; rr < topLeft.first+3; ++rr) {
        for (int cc = topLeft.second; cc < topLeft.second+3; ++cc) {
            if (rr == row && cc == col1) continue; // original location of digit1
            if (rr == row && cc == col2) continue; // skip itself
            if (board.board[rr][cc] == digit1) return false;
        }
    }


    return true;
}

// Taken from original genetic algo code. Assume called when a mutation chance is procced
// paremeterized for whether it checks if the swap is an improving swap (see isLegalSwap() comment)
SudokuBoard rowSwapBase(SudokuBoard newBoard, bool check) {


    for (int row = 0; row < 9; ++row) {
    
        // Find mutable (non-given) columns for this row
        std::vector<int> mutableCols;
        for (int col = 0; col < 9; ++col) {
            bool isGiven = false;
            for (const auto& givensIterator : newBoard.givens) {
                if (givensIterator[0] == row && givensIterator[1] == col) {
                    isGiven = true;
                    break;
                }
            }
            if (!isGiven) {
                mutableCols.push_back(col);
            }
        }

        // If fewer than 2 mutable cells, skip this row
        int availableCols = mutableCols.size();
        if (availableCols < 2) {
            continue;
        }
        // Randomly pick two different mutable columns to swap
        int index1 = rand() % availableCols;  
        int index2 = rand() % availableCols;  
        while (index2 == index1) { // reroll dupes
            index2 = rand() % availableCols;  
        }

        int col1 = mutableCols[index1];
        int col2 = mutableCols[index2];

    // Swap them
        if (check) {
            if (isLegalSwap(newBoard, row, col1, col2)) {
                std::swap(newBoard.board[row][col1], newBoard.board[row][col2]);
            }
        }
        
    

        
        else {
            continue;
        }
    }
    
    return newBoard;
}

SudokuBoard rowSwap(SudokuBoard newBoard) {
    return rowSwapBase(newBoard, false);
}

SudokuBoard rowSwapCheck(SudokuBoard newBoard) {
    return rowSwapBase(newBoard, true);
}