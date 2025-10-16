#include "class_sudoku.cpp"
#include "board_loader.cpp"
#include "brute_force.cpp"
using namespace std;

int main() {
    srand(6); // set the seed
    SudokuBoard board = getRandomBoard();
    
    //tests valid function
    board.print();
    cout << "solving...\n";
    SudokuBoard solvedBoard = bruteForce(board);
    solvedBoard.print();

    if (solvedBoard.checkValid()) {
        cout << "valid\n";
    }
    else {
        cout << "nonvalid\n";
        cout << solvedBoard.fitness << "\n";

    }
}