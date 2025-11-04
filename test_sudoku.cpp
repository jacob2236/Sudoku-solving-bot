#include <iostream>
#include "class_sudoku.cpp"
#include "class_sudoku_possibilities.cpp"
#include "board_loader.cpp"
#include "brute_force.cpp"
#include "tabu_search.cpp"
using namespace std;



int main() {
    srand(1); // set the seed
    SudokuBoard board = getRandomBoard();
    
    //tests valid function
    board.print();
    cout << "solving...\n";
    PossibilitiesBoard solvedBoard = tabuSearch(board, 10000, 100, 100);
    solvedBoard.print();
    if (solvedBoard.checkValid()) {
        cout << "valid\n";
    }
    else {
        cout << "nonvalid\n";

    }


    board = getRandom17Board();
    
    //tests valid function
    board.print();
    cout << "solving...\n";
    solvedBoard = tabuSearch(board, 10000, 100, 100);
    solvedBoard.print();
    if (solvedBoard.checkValid()) {
        cout << "valid\n";
    }
    else {
        cout << "nonvalid\n";
    }

    board = getRandomHardBoard();
    
    //tests valid function
    board.print();
    cout << "solving...\n";
    solvedBoard = tabuSearch(board, 10000, 100, 100);
    solvedBoard.print();
    if (solvedBoard.checkValid()) {
        cout << "valid\n";
    }
    else {
        cout << "nonvalid\n";
    }

    cout << endl; // flush buffer
    return 0;


}