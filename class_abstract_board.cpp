#include "class_sudoku.cpp"
#include "class_represented_board.cpp"
#include <functional>
#pragma once

class AbstractBoard{
    public:
        function<int(SudokuBoard)> fitness;
        function<SudokuBoard(SudokuBoard)> mutate; // takes just a Sudokuboard to be representation agnostic
        function<SudokuBoard(SudokuBoard, SudokuBoard)> mix;
        RepresentedBoard repBoard;
        int fitnessValue = -1; // negative implies no fitness evaluation attempted. Don't try to compare without evaluation
        AbstractBoard(function<int(SudokuBoard)> fit, function<SudokuBoard(SudokuBoard)> mut, 
                        function<SudokuBoard(SudokuBoard, SudokuBoard)> cross, RepresentedBoard board) 
                        : fitness(fit), mutate(mut), mix(cross), repBoard(board){}
        AbstractBoard(){}
        AbstractBoard sample() {
            //returns a NEW AbstractBoard with a mutated board (as per the given mutation function)
            // MUST be a valid board

            // loop initiaty bits
            bool valid = false;
            SudokuBoard mutableCopy = repBoard.base;
            do {
                // mutate and check until we have a valid mutation
                mutableCopy = mutate(repBoard.base);
                valid = repBoard.validate(mutableCopy);
            } 
            while (!valid);
            //We have a valid mutation. Make a new rep out of it and make the new total abstract board
            auto newRep = repBoard.newRep(mutableCopy); // Each repboard will have its own newRep() for its type
            AbstractBoard nextBoard = AbstractBoard(fitness, mutate, mix, newRep);
            return nextBoard;           
        }

        int getFitness() {
            return fitness(repBoard.base);
        }

        AbstractBoard crossover(AbstractBoard otherBoard) {
            // like sample but using the crossover function
            bool invalid = true;
            SudokuBoard mutableCopy = repBoard.base;
            do {
                mutableCopy = mix(repBoard.base, otherBoard.repBoard.base);
                invalid = repBoard.validate(mutableCopy);
            } while (invalid);
            auto newBoard = repBoard.newRep(mutableCopy); //TODO Repboard typing!!!
            return AbstractBoard(fitness, mutate, mix, newBoard);
        }

        void print(){
            repBoard.base.print();
            bool validSudoku = repBoard.base.checkValid();
            bool validRep = repBoard.validate(repBoard.base);
            int fitnessVal = getFitness();
            cout << "\n Validity = ";
            cout << validSudoku;
            cout << " Valid representation ";
            cout << validRep;
            cout << "\n Fitness = ";
            cout << fitnessVal;
            cout << "\n";
        }
};