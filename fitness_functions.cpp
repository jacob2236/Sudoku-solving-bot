#include "class_sudoku.cpp"
#include <set>
#include <functional>
/* This contains all fitness functions which can be passed into an abstract board
    All of these take a normal SudokuBoard and return an int representing fitness
    For consistency's sake, all fitness functions should be MINIMIZING 
    The minimum value representing a perfectly solved board must be 0.
*/

int countConflicts(SudokuBoard currentBoard) {
    // Counts the number of duplicates in each row, col, and group
    // A cell can be counted multiple times if it is a conflict in a row AND a column etc
    int conflicts = 0;
    auto getDuplicates = [&conflicts](int cells[9]) {
        set<int> foundDigits = {};
        for(int i = 0; i < 9; i++) {
            int digit = cells[i];
            foundDigits.insert(digit);
        }
        conflicts += 9 - foundDigits.size();
    };
    currentBoard.forEachRow(getDuplicates);
    currentBoard.forEachCol(getDuplicates);
    currentBoard.forEachGroup(getDuplicates);
    return conflicts;
}

int countPossibilities(SudokuBoard currentBoard) {
    // returns the number of possibilities in unfilled cells
    // PLUS the number of unfilled cells
    // This is to prevent a board with 1 or more empty cells with no possibilities from scoring 0
    // Since 0 is reserved exclusively for fully solved boards.

    auto getPossibilities = [](const SudokuBoard &inputBoard, int row, int col) {// copied fresh from possibilitiesboard
        set<int> digits = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for(int i = 0; i < 9; i++) {
            int rowDigit = inputBoard.board[row][i];
            if(i != col && rowDigit != 0) {
                digits.erase(rowDigit);
            }
            int colDigit = inputBoard.board[i][col];
            if(i != row && colDigit != 0) {
                digits.erase(colDigit);
            }

        }
        int top = (row / 3) * 3; //get the top left corner of the group the cell is in
        int left = (col / 3) * 3;
        for(int y = 0; y < 3; y++) {
            for(int x = 0; x < 3; x++) {
                int groupDigit = inputBoard.board[top + y][left + x];
                if(row != top + y && col != left + x && groupDigit != 0) {
                    digits.erase(groupDigit);
                }
            }
        }
        return digits.size() + 1; // +1 to account for empty cells with no possibilities
    };

    int possibilities = 0;
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++) {
            if(currentBoard.board[row][col] == 0) {
                possibilities += getPossibilities(currentBoard, row, col);
            }
        }
    }
    return possibilities;

}