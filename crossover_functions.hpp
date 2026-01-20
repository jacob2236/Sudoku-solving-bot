#include "class_sudoku.hpp"

#ifndef CROSSOVER_FUNCTIONS 
#define CROSSOVER_FUNCTIONS

SudokuBoard dummy (const SudokuBoard &first, const SudokuBoard &second);
std::array<int,9> permutationRow(const std::array<int,9>& parent1, const std::array<int,9>& parent2);
SudokuBoard permutationRowCrossover(const SudokuBoard &first, const SudokuBoard &second);
std::array<int,9> cycleCrossoverRow(const std::array<int,9>& parent1,
                                    const std::array<int,9>& parent2);
SudokuBoard cycleCrossover(const SudokuBoard &parent1, const SudokuBoard &parent2);
std::array<int,9> orderCrossoverRow(const std::array<int,9>& parent1,
                                    const std::array<int,9>& parent2);
SudokuBoard orderCrossoverPerRow(const SudokuBoard &parent1,
                                 const SudokuBoard &parent2);   
SudokuBoard rowWiseCrossover(const SudokuBoard &parent1, const SudokuBoard &parent2); 
SudokuBoard rowRandomCrossover(const SudokuBoard &parent1, const SudokuBoard &parent2);                                                                 
#endif