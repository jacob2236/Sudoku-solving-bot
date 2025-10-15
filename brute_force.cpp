#include "class_sudoku.h"

/* To compare metaheuristics we should have a very simple heuristic example to compare off of.
Some of these algorithms give poor results or appear poorly suited for sudoku.
It would be valuable to see which are better or worse than brute-force checking for duplicates.*/

SudokuBoard bruteForce(SudokuBoard board) {
    for(int row = 0; row <9; row++) {
        for(int col = 0; col <9; col++) {
            //search in row-major order for the first empty cell
            if(board.get(row, col) == 0) {
                for(int attempt = 1; attempt < 10; attempt++) {
                    //try placing all 9 digits into that cell
                    board.add(row, col, attempt);
                    if(!board.checkValidCell(row, col)) {
                        continue; // don't recurse if we know this cell's bad.
                    }
                    //recurse to solve other blank spaces
                    SudokuBoard potential = bruteForce(board);
                    //if any board down the line returns a valid board, pass it upwards!
                    if(potential.checkValid()) {
                        return potential;
                    }
                }
                //return the invalid board if no digit can validly fill that cell
                return board;
            }
        }
    }
    //if no empty cell has been found, return the (hopefully) valid board
    return board;
}