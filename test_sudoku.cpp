#include <iostream>
#include "class_sudoku.h"
#include "board_loader.cpp"
using namespace std;



int main() {
    srand(600); // set the seed
    SudokuBoard board = getRandomBoard();
    
    //tests valid function
    board.print();
    if (board.checkValid()) {
        cout << "valid\n";
    }
    else {
        cout << "nonvalid\n";
    }


    board = getRandom17Board();
    
    //tests valid function
    board.print();
    if (board.checkValid()) {
        cout << "valid\n";
    }
    else {
        cout << "nonvalid\n";
    }

    board = getRandomHardBoard();
    
    //tests valid function
    board.print();
    if (board.checkValid()) {
        cout << "valid\n";
    }
    else {
        cout << "nonvalid\n";
    }

    cout << endl; // flush buffer
    return 0;


}