#include <iostream>
#include "class_sudoku.hpp"
#include "board_loader.hpp"

#include "algorithm_brute_force.hpp"
#include "algorithm_tabu_search.hpp"
#include "algorithm_genetic.hpp"

#include "class_abstract_board.hpp"
#include "mutation_functions.hpp"
#include "fitness_functions.hpp"
#include "class_represented_rowconsistent.hpp"
#include "crossover_functions.hpp"
#include <fstream>
using namespace std;



int main() {
    srand(1); // set the seed

    //Data collection

    //DOUBLE CHECK AND SAVE OLD STUFF BEFORE EACH RUN
    ofstream fileWriter("solver_data/testrunoutput.csv");
    fileWriter << "Board, Trial, Time, Fitness, solved\n";
    fileWriter << "\t, average time, average fitness, average solved\n";

    double average_time = 0; 
    double average_solve = 0;
    double average_fitness = 0;
    double total_time = 0;
    double total_fitness = 0;
    int boards_solved = 0;
    int boards_per_study = 20;
    int num_trials_per_board = 5;
    clock_t start, end;
    for (int board_count = 1; board_count < boards_per_study; board_count++ ) {
        SudokuBoard initialBoard = getRandomBoard();
        cout << "New board for 5 trials! This is board number " << board_count << "\n";
        initialBoard.print();
        for (int trial=1;trial<=num_trials_per_board;trial++) {
            start = clock();

            // DO tests
            // give it to a representation
            RepresentedRowConsistent repBoard(initialBoard);
            // make the abstract board
            AbstractBoard abstractBoard(countConflicts, rowSwap, rowWiseCrossover, repBoard);
            cout << "solving...\n";
            AbstractBoard solvedBoard = geneticAlgorithm(abstractBoard, .6, 0.8);
            solvedBoard.print();

            end = clock();
            double time_taken = double(end - start);
            int fitness = solvedBoard.getFitness();
            cout << "Attempt #" << trial << " trial\n";
            cout << "Best fitness: " << fitness << " trial\n";
            cout << "Time(CPU ticks): " << time_taken << " trial\n";
            total_time = total_time + time_taken;
            total_fitness += fitness;
            fileWriter << board_count << ", " << trial << ", " << time_taken << ", " << fitness << ", ";
            if (fitness == 0) {
                boards_solved = boards_solved + 1;
                fileWriter << "1\n";
            } else {
                fileWriter << "0\n";
            }
            
            
            cout << "--------------------------------------trial\n";
        }
        average_time = total_time / (5.0 * board_count);
        average_fitness = total_fitness / (5.0 * board_count);
        average_solve = boards_solved / (5.0 * board_count);
        fileWriter << "\t," << average_time << ", " << average_fitness << ", " << average_solve << "\n";
        cout << "average time: " << average_time;
        cout << "\naverage fitness: " << average_fitness;
        cout << "\naverage solved: " << average_solve;
        cout.flush();

    }
    

    


}