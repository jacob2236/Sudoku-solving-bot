#include "class_abstract_board.hpp"
#ifndef ALGORITHM_TABU_SEARCH 
#define ALGORITHM_TABU_SEARCH

int rand9();

array<int, 3> getDifference(AbstractBoard& newBoard, AbstractBoard& originak);

AbstractBoard tabuSearch(AbstractBoard inputBoard, int maxIter, int tabuLength);
#endif