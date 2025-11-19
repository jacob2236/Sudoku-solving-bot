#include "class_sudoku.cpp"
#include "class_represented_board.cpp"
#include <functional>

class AbstractBoard{
    public:
        function<int(SudokuBoard)> fitness;
        function<SudokuBoard(SudokuBoard)> mutate; // takes just a Sudokuboard to be representation agnostic
        RepresentedBoard repBoard;
        AbstractBoard(function<int(SudokuBoard)> fit, function<SudokuBoard(SudokuBoard)> mut, RepresentedBoard board) {
            fitness = fit;
            mutate = mut;
            repBoard = board;
        }

        AbstractBoard sample() {
            //returns a NEW AbstractBoard with a mutated board (as per the given mutation function)
            RepresentedBoard* newBoard = NULL;
            do {
                SudokuBoard mutableCopy = mutate(repBoard.base);
                newBoard = repBoard.validate(mutableCopy);
            } while (newBoard != NULL);
            return AbstractBoard(fitness, mutate, *newBoard);           
        }
};