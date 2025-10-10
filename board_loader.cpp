#include "class_sudoku.cpp"
#include <fstream>

/* Code to read boards from a given dataset and turn them into SudokuBoard objects
   While I downloaded the whole set of sets I think I only intend to use the unbiased and 17-clue sets
   And maybe hardest 1905 11+, which is purported to contain puzzles deemed by a broader Sudoku community to be extremely difficult
*/

//Total number of boards per sudoku dataset
int puzzle01MaxSize = 1000000;
int puzzle2MaxSize = 49158;
int puzzle5MaxSize = 48766;

SudokuBoard getSpecificBoard(int index){
    ifstream fileReader("data/puzzles1_unbiased"); //instantiate a read-only filereader to only read the file
    if(index != 0) {
        index -= 1; // due to oddities with the reading system it actually reads the index+1-th line. This makes sure we don't go out of bounds
    }
    fileReader.seekg(82*index); //magic number 82 because each line is 82 chars long including linebreak
    char boardString[81]; // string to be a destination for getline
    fileReader.getline(boardString, 82); //stops on the 82nd character (IE linebreak)
    fileReader.close();
    return SudokuBoard(string(boardString));

}

SudokuBoard getRandomBoard() {
    double randomValue = (double)std::rand() / RAND_MAX;
    int index = randomValue * puzzle01MaxSize;
    return getSpecificBoard(index);
}



SudokuBoard getSpecific17Board(int index) {
    ifstream fileReader("data/puzzles2_17_clue"); //instantiate a read-only filereader to only read the file
    if(index != 0) {
        index -= 1; 
    }
    fileReader.seekg(82*index); //magic number 82 because each line is 82 chars long including linebreak
    char boardString[81]; // string to be a destination for getline
    fileReader.getline(boardString, 82); //stops on the 82nd character (IE linebreak)
    fileReader.close();
    return SudokuBoard(string(boardString));
}

SudokuBoard getRandom17Board() {
    double randomValue = (double)std::rand() / RAND_MAX;
    int index = randomValue * puzzle2MaxSize;
    return getSpecific17Board(index);
}


SudokuBoard getSpecificHardBoard(int index){
    ifstream fileReader("data/puzzles5_forum_hardest_1905_11+"); //instantiate a read-only filereader to only read the file
    if(index != 0) {
        index -= 1; // There's some lines of text with sources at the top
    }
    fileReader.seekg(82*index); //magic number 82 because each line is 82 chars long including linebreak
    char boardString[81]; // string to be a destination for getline
    fileReader.getline(boardString, 82); //stops on the 82nd character (IE linebreak)
    fileReader.close();
    return SudokuBoard(string(boardString));
}

SudokuBoard getRandomHardBoard(){
    double randomValue = (double)std::rand() / RAND_MAX;
    int index = randomValue * puzzle5MaxSize;
    return getSpecificHardBoard(index);
}
