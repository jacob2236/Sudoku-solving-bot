#include "class_sudoku.cpp"
#include <set>
using namespace std;
#pragma once

/* A representation of the Sudokuboard that comes with a list of possible digits for each empty cell.
Useful for algorithms that solve boards by building up a solution rather than generating random solutions.*/

class PossibilitiesBoard: public SudokuBoard {
    public:
        set<int> possibilities[9][9];
        PossibilitiesBoard(string boardString){ //takes an 81 char string representing the board. '.' represents an empty space
            auto nextChar = boardString.begin(); // make an iterator to pass through the string representation of the board
            for(int row = 0; row < 9; row++) {
                for(int col = 0; col < 9; col++) {
                    if(*nextChar == '.') {
                        board[row][col] = 0;
                    } else{
                        board[row][col] = *nextChar - ascii0; // subtract
                    }
                    nextChar++;
                }
            }
            // Now that the board is built, build the possibilities board
            for(int row = 0; row < 9; row++) {
                for(int col = 0; col < 9; col++) {
                    if(board[row][col] != 0) {
                        possibilities[row][col].insert(-1); // -1 marks the cell as a given
                    } else {
                        possibilities[row][col] = getPossibilities(row, col);
                    }
                }
            }
        }
        PossibilitiesBoard(const SudokuBoard& copyBoard, set<int> possibilities[][9]) {
            // construct from a copy of another board rather than a fresh string. Good for mutations
            // checks past possibilities to deliniate between givens and mutant cells
            for (int row = 0; row < 9; row++){
                for(int col = 0; col < 9; col++){
                    board[row][col] = copyBoard.board[row][col];
                }
            }
            for(int row = 0; row < 9; row++) {
                for(int col = 0; col < 9; col++) {
                    if(board[row][col] != 0) {
                        if(possibilities[row][col].count(-1)){
                            possibilities[row][col].insert(-1); // -1 marks the cell as a given
                        } else {
                            continue; // no possibilities for filled cells
                        }
                    } else {
                        possibilities[row][col] = getPossibilities(row, col);
                    }
                }
            }
        }
        PossibilitiesBoard(const SudokuBoard& copyBoard) {
            // construct from a copy of another board rather than a fresh string. easy construction
            for (int row = 0; row < 9; row++){
                for(int col = 0; col < 9; col++){
                    board[row][col] = copyBoard.board[row][col];
                }
            }
            for(int row = 0; row < 9; row++) {
                for(int col = 0; col < 9; col++) {
                    if(board[row][col] != 0) {
                        possibilities[row][col].insert(-1); // -1 marks the cell as a given
                    } else {
                        possibilities[row][col] = getPossibilities(row, col);
                    }
                }
            }
        }
        
        PossibilitiesBoard(){}
        set<int> getPossibilities(int row, int col) {
            set<int> digits = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            for(int i = 0; i < 9; i++) {
                int rowDigit = board[row][i];
                if(i != col && rowDigit != 0) {
                    digits.erase(rowDigit);
                }
                int colDigit = board[i][col];
                if(i != row && colDigit != 0) {
                    digits.erase(colDigit);
                }

            }
        int top = (row / 3) * 3; //get the top left corner of the group the cell is in
        int left = (col / 3) * 3;
        for(int y = 0; y < 3; y++) {
            for(int x = 0; x < 3; x++) {
                int groupDigit = board[top + y][left + x];
                if(row != top + y && col != left + x && groupDigit != 0) {
                    digits.erase(groupDigit);
                }
            }
        }
        return digits;
        }

        PossibilitiesBoard mutate() {
            // do
            // random row
            // random col
            // while that spot isn't a given
            //If it's filled, make a new board with it unfilled
            // if it's unfilled pick a random possibility to make a fresh board with that filled in
            int randRow;
            int randCol;
            do {
                randRow = rand() % 9;
                randCol = rand() % 9;
                set<int> cellPossibilities = possibilities[randRow][randCol];
                if(cellPossibilities.count(-1) == 0){
                    // -1 is not in the possibility set, this isn't a given. 
                    if(board[randRow][randCol] == 0) {
                        // if the board is empty at this space, let's fill it
                        if(cellPossibilities.empty()) {
                            continue; // no possibilities to fill with? Nothing to do. Try again
                        } 
                        int randDigit = getRandomPossibility(cellPossibilities);
                        SudokuBoard newBoard = SudokuBoard(*this);
                        newBoard.add(randRow, randCol, randDigit);
                        return PossibilitiesBoard(newBoard, possibilities);
                    } else {
                        // there exists a cell at this space. Remove it
                        SudokuBoard newBoard = SudokuBoard(*this);
                        newBoard.del(randRow, randCol);
                        return PossibilitiesBoard(newBoard, possibilities);
                    }
                }
            } while (true); // repeat if there's a failure

        }

        int getRandomPossibility(set<int> choices) {
            //kinda gross code so I figured I'd get it out of the mutate function for sanity's sake
            int randChoice = rand() % choices.size(); // select a random member of the set
            int count = 0;
            for (int possibility : choices) {
                if(count == randChoice) {
                    return possibility;
                }
                count++;
            }
            return 0; // fairly harmless fail condition but this SHOULD be unreachable
        }
        int fitness(){
            int filledCells = 0;
            for(int row = 0; row < 9; row++){
                for(int col = 0; col < 9; col++){
                    if(board[row][col] != 0) {
                        filledCells++;
                    }
                }
            }
            return filledCells;
        }
        
};