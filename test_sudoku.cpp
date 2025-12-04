#include <iostream>
#include "class_sudoku.cpp"
#include "class_sudoku_possibilities.cpp"
#include "board_loader.cpp"
#include "brute_force.cpp"
#include "tabu_search.cpp"
#include "class_abstract_board.cpp"
#include "mutation_functions.cpp"
#include "fitness_functions.cpp"
#include "class_represented_alldifferent.cpp"
#include "crossover_functions.cpp"
using namespace std;



int main() {
    srand(1); // set the seed
    SudokuBoard board = getRandomBoard();
    // give it to a representation
    RepresentedAllDifferent repBoard(board);
    // make the abstract board
    AbstractBoard abstractBoard(countConflicts, fillPossibilityOrRemove, dummy, repBoard);
    //test
    board.print();
    cout << "solving...\n";
    AbstractBoard solvedBoard = randomBruteForce(abstractBoard, 1000);
    solvedBoard.print();



    board = getRandom17Board();
    
    // give it to a representation
    repBoard = RepresentedAllDifferent(board);
    // make the abstract board
    abstractBoard = AbstractBoard(countConflicts, fillPossibilityOrRemove, dummy, repBoard);
    //test
    board.print();
    cout << "solving...\n";
    solvedBoard = randomBruteForce(abstractBoard, 1000);
    solvedBoard.print();


    board = getRandomHardBoard();
    
    // give it to a representation
    repBoard = RepresentedAllDifferent(board);
    // make the abstract board
    abstractBoard = AbstractBoard(countConflicts, fillPossibilityOrRemove, dummy, repBoard);
    //test
    board.print();
    cout << "solving...\n";
    solvedBoard = randomBruteForce(abstractBoard, 1000);
    solvedBoard.print();

    cout << endl; // flush buffer
    return 0;


}