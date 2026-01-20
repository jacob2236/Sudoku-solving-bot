#include "class_sudoku.hpp"
#include "class_represented_board.hpp"
#include <functional>
#include <iostream>

using namespace std;
#ifndef ABSTRACT_BOARD 
#define ABSTRACT_BOARD
class AbstractBoard {
    public:
        function<int(SudokuBoard)> fitness;
        function<SudokuBoard(SudokuBoard)> mutate; // takes just a Sudokuboard to be representation agnostic
        function<SudokuBoard(SudokuBoard, SudokuBoard)> mix;
        RepresentedBoard repBoard;
        int fitnessValue = -1; // negative implies no fitness evaluation attempted. Don't try to compare without evaluation
        AbstractBoard(function<int(SudokuBoard)> fit, function<SudokuBoard(SudokuBoard)> mut, 
                function<SudokuBoard(SudokuBoard, SudokuBoard)> cross, RepresentedBoard board);
        AbstractBoard();
        AbstractBoard sample();
        int getFitness();
        AbstractBoard crossover(AbstractBoard otherBoard);
        SudokuBoard getBoard();
        void print();



};
#endif