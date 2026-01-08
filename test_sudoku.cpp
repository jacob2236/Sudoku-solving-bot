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

    //Data collection
    double average_time = 0; 
    double average_solve = 0;
    int boards_solved = 0;
    int num_of_boards = 20;
    int num_trials_per_board = 5;
    clock_t start, end;

    for (int trial=1;trial<=num_trials_per_board;trial++) {
        const std::clock_t c_start = std::clock();
        start = clock();

        // DO tests

        end = clock();
        double time_taken = double(end - start);
        cout << "Attempt #" << trial << "\trial";
        cout << "Best fitness: " << generation[0].fitness << "\trial";
        cout << "Time(CPU ticks): " << time_taken << "\trial";
        cout << "--------------------------------------\trial";
    }


}