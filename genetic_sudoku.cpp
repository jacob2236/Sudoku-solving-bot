#include "class_sudoku.cpp"
#include "board_loader.cpp"
#include "brute_force.cpp"
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

struct Coordinate {
    int row, col,num;
    Coordinate(int r, int c, int num) : row(r), col(c), num(num) {}
};

void merge(SudokuBoard gen[], int left, 
                     int mid, int right){
                         
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp vectors
    SudokuBoard L[n1], R[n2];

    // Copy data to temp vectors L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = gen[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = gen[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    // Merge the temp vectors back 
    // into arr[left..right]
    while (i < n1 && j < n2) {
        if (L[i].fitness <= R[j].fitness) {
            gen[k] = L[i];
            i++;
        }
        else {
            gen[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], 
    // if there are any
    while (i < n1) {
        gen[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], 
    // if there are any
    while (j < n2) {
        gen[k] = R[j];
        j++;
        k++;
    }
}

// classic mergesort sorting based on fitness of each board
void mergeSort(SudokuBoard gen[], int left, int right){
    
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(gen, left, mid);
    mergeSort(gen, mid + 1, right);
    merge(gen, left, mid, right);
}

// used for crossover function, assigned a new board the first 5 rows of temp1 board,
// and then assigns last 4 rows of temp2 board to the new board
SudokuBoard cross(SudokuBoard temp1,SudokuBoard temp2) {
    SudokuBoard answer;
    for (int i=0;i<9;i++) {
        if (i%2 == 0) {
            for (int j=0;j<9;j++) {
            answer.board[i][j] = temp2.board[i][j];
            }
        }
        else {
            for (int j=0;j<9;j++) {
            answer.board[i][j] = temp1.board[i][j];
            }
        }
    }
    
    return answer;
}

// mutation function iterates through each row of the sudoku board, it checks the givens vector to make sure
// it is not switching given values, randomly picks nongivens to be swapped and swaps them keeping row consistency
SudokuBoard mutate(SudokuBoard board, const std::vector<Coordinate>& givens,std::mt19937 &rng,double mutate_percentage) {

    std::uniform_real_distribution<> rnd01(0.0, 1.0);


    for (int row = 0; row < 9; ++row) {
        double r = rnd01(rng);
        if (r < mutate_percentage) {
            // Find mutable (non-given) columns for this row
            std::vector<int> mutableCols;
            for (int col = 0; col < 9; ++col) {
                bool isGiven = false;
                for (const auto& g : givens) {
                    if (g.row == row && g.col == col) {
                        isGiven = true;
                        break;
                    }
                }
                if (!isGiven) {
                    mutableCols.push_back(col);
                }
            }

            // If fewer than 2 mutable cells, skip this row
            if (mutableCols.size() < 2) continue;

            // Randomly pick two different mutable columns to swap
            std::uniform_int_distribution<> dist(0, mutableCols.size() - 1);
            int idx1 = dist(rng);  //gen
            int idx2 = dist(rng);  //gen
            while (idx2 == idx1) {
                idx2 = dist(rng);  //gen
            }

            int c1 = mutableCols[idx1];
            int c2 = mutableCols[idx2];

            // Swap them
            std::swap(board.board[row][c1], board.board[row][c2]);
        }
        else {
            continue;
        }
    }
    
    return board;
}

// returns a vector of givens for the board
std::vector<Coordinate> givens(SudokuBoard board) {
    std::vector<Coordinate> coordinates;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board.board[i][j] != 0) {
                coordinates.push_back(Coordinate(i, j, board.board[i][j]));
            }
        }
    }
    return coordinates;
}

// randomly fills board keeping row consitency
SudokuBoard randomFillRows(SudokuBoard board) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < 9; ++i) {
        // Track numbers already present in the row
        bool used[10] = {false}; // 1–9
        for (int j = 0; j < 9; ++j) {
            if (board.board[i][j] != 0) {
                used[board.board[i][j]] = true;
            }
        }

        // Collect missing numbers
        std::vector<int> missing;
        for (int num = 1; num <= 9; ++num) {
            if (!used[num]) missing.push_back(num);
        }

        // Shuffle missing numbers randomly
        std::shuffle(missing.begin(), missing.end(), gen);

        // Fill empty cells with shuffled missing numbers
        int k = 0;
        for (int j = 0; j < 9; ++j) {
            if (board.board[i][j] == 0) {
                board.board[i][j] = missing[k];
                k++;
            }
        }
    }
    return board;
}

int main() {
    int num_of_samples = 1000;
    int num_of_generations = 100; // future use for how many times we want to run generations
    SudokuBoard generation[num_of_samples]; // size of generation can change based on our input
    double mutate_percentage = .6; // determines how much of the time we mutate a board 60% as per paper 9
    std::mt19937 rng(12345); // random seed
    std::vector<Coordinate> coordinates;
    SudokuBoard board_to_solve = getRandomBoard();
    SudokuBoard board;

    // board generation for the very first generation
    for (int i=0;i < num_of_samples;i++) {
        board = randomFillRows(board_to_solve);
        generation[i] = board;
    }

    // calls givens to get the vector of givens for given board
    coordinates = givens(board_to_solve);

    // best board kept to be printed at the end of the program
    SudokuBoard best_board = generation[0];
    best_board.checkValid();

    // running number of generations
    for (int n=0;n<num_of_generations;n++) {
        // assigning fitness and sorting
        for (int i=0;i < num_of_samples;i++) {
            generation[i].checkValid();
        }
        mergeSort(generation, 0, num_of_samples - 1);

        // testing each sorted generation
        //cout << "This prints the best fitness score for the " << (n+1) << "th generation: " << generation[0].fitness << "\n";

        // crossover, keeping first half of original generation while crossing the 
        // third 25% with the final 25%
        int half_samples = num_of_samples/2;
        int quarter_samples = num_of_samples/4;
        int randnum1 = 0;
        int randnum2 = 0;
        SudokuBoard temp1,temp2;
        for (int i=half_samples;i<num_of_samples;i++) {
            randnum1 = rand()%(quarter_samples) + 0;
            randnum2 = rand()%(quarter_samples) + quarter_samples;
            temp1 = generation[randnum1];
            temp2 = generation[randnum2];
            generation[i] = cross(temp1,temp2);
        }

        // mutation function takes all the samples and mutates them according to a percentage
        // we can give and edit the value of
        for (int i=quarter_samples;i<num_of_samples;i++) {
           generation[i] = mutate(generation[i],coordinates,rng,mutate_percentage);
        }

        // reassign best board if there is a better board
        if (generation[0].fitness < best_board.fitness) {
            best_board = generation[0];
        }
    }
    
     //prints best board we got through the generations called
    cout << "This is the best fitness score produced given " 
    << num_of_generations << " generations: " << best_board.fitness << "\n"; 
}
