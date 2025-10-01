#include <iostream>
#include "class_sudoku.cpp"
using namespace std;



int main() {
    SudokuBoard board;
    int row, column, num;
    char answer;
    
    //example board
    board.add(0,0,5);
    board.add(0,1,3);
    board.add(0,2,4);
    board.add(0,3,6);
    board.add(0,4,7);
    board.add(0,5,8);
    board.add(0,6,9);
    board.add(0,7,1);
    board.add(0,8,2);

    board.add(1,0,6);
    board.add(1,1,7);
    board.add(1,2,2);
    board.add(1,3,1);
    board.add(1,4,9);
    board.add(1,5,5);
    board.add(1,6,3);
    board.add(1,7,4);
    board.add(1,8,8);

    board.add(2,0,1);
    board.add(2,1,9);
    board.add(2,2,8);
    board.add(2,3,3);
    board.add(2,4,4);
    board.add(2,5,2);
    board.add(2,6,5);
    board.add(2,7,6);
    board.add(2,8,7);

    board.add(3,0,8);
    board.add(3,1,5);
    board.add(3,2,9);
    board.add(3,3,7);
    board.add(3,4,6);
    board.add(3,5,1);
    board.add(3,6,4);
    board.add(3,7,2);
    board.add(3,8,3);

    board.add(4,0,4);
    board.add(4,1,2);
    board.add(4,2,6);
    board.add(4,3,8);
    board.add(4,4,5);
    board.add(4,5,3);
    board.add(4,6,7);
    board.add(4,7,9);
    board.add(4,8,1);

    board.add(5,0,7);
    board.add(5,1,1);
    board.add(5,2,3);
    board.add(5,3,9);
    board.add(5,4,2);
    board.add(5,5,4);
    board.add(5,6,8);
    board.add(5,7,5);
    board.add(5,8,6);

    board.add(6,0,9);
    board.add(6,1,6);
    board.add(6,2,1);
    board.add(6,3,5);
    board.add(6,4,3);
    board.add(6,5,7);
    board.add(6,6,2);
    board.add(6,7,8);
    board.add(6,8,4);

    board.add(7,0,2);
    board.add(7,1,8);
    board.add(7,2,7);
    board.add(7,3,4);
    board.add(7,4,1);
    board.add(7,5,9);
    board.add(7,6,6);
    board.add(7,7,3);
    board.add(7,8,5);

    board.add(8,0,3);
    board.add(8,1,4);
    board.add(8,2,5);
    board.add(8,3,2);
    board.add(8,4,8);
    board.add(8,5,6);
    board.add(8,6,1);
    board.add(8,7,7);
    board.add(8,8,9);

    //tests valid function
    board.print();
    if (board.checkValid()) {
        cout << "valid";
    }
    else {
        cout << "nonvalid";
    }
    return 0;
}