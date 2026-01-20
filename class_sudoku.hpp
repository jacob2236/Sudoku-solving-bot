#include <string>
#include <array>
#include <vector>
#include <functional>

#pragma once
#ifndef SUDOKU_BOARD
#define SUDOKU_BOARD

using namespace std;
class SudokuBoard{
    public:
        int ascii0 = '0'; // integer value of the ascii character 0. Useful for converting the ascii digits to int digits
        int fitness;
        array<array<int, 9>, 9> board;
        vector<array<int, 3>> givens;
        SudokuBoard(string boardString);
        SudokuBoard();
    void print();
    bool checkValid();
    bool checkValidCell(int row, int col);
    void add(int row, int column, int num);
    void del(int row, int column);
    int get(int row, int col);
    void forEachCell(function<void(int)> func);
    void forEachCol(function<void(int[9])> func);
    void forEachRow(function<void(int[9])> func);
    void forEachGroup(function<void(int[9])> func);
    void forThisGroup(function<void(int[9])> func, int row, int col);
};

#endif