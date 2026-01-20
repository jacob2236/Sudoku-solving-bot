#include "class_abstract_board.hpp"

#ifndef ALGORITHM_GENETIC
#define ALGORITHM_GENETIC

struct Coordinate{
    int row, col, num;
    Coordinate(int r, int c, int num);
};

void merge(AbstractBoard gen[], int left, 
                     int mid, int right);

void mergeSort(AbstractBoard gen[], int left, int right);
int roulettePick(const std::vector<double>& cumWeights);
AbstractBoard geneticAlgorithm(AbstractBoard inputBoard, int maxIter, double mutationChance, double crossoverChance);
#endif