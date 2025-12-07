#include "class_sudoku.cpp"
#include "board_loader.cpp"
#include "brute_force.cpp"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

//used for givens in the board
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

// parents and child rows as arrays of int[9]
std::array<int,9> pmx_row(const std::array<int,9>& p1, const std::array<int,9>& p2, std::mt19937 &rng) {
    std::array<int,9> child;
    child.fill(-1);
    std::uniform_int_distribution<> cutDist(0,8);
    int c1 = cutDist(rng);
    int c2 = cutDist(rng);
    if (c1 > c2) std::swap(c1,c2);
    // copy segment from p1
    for (int i=c1;i<=c2;i++) child[i] = p1[i];

    // mapping from p2 segment into child
    for (int i=c1;i<=c2;i++) {
        int val = p2[i];
        // if val not already in child segment, place it
        bool present=false;
        for (int j=c1;j<=c2;j++) if (child[j]==val) present=true;
        if (!present) {
            int pos = i;
            int mapped = val;
            // resolve position by following mapping p2[pos] -> p1[pos]
            while (true) {
                int v = p1[pos];
                // find index of v in p2
                int idx = -1;
                for (int k=0;k<9;k++) if (p2[k]==v) { idx = k; break; }
                pos = idx;
                if (pos < c1 || pos > c2) break;
            }
            // pos is a free position
            child[pos] = val;
        }
    }

    // fill remaining positions with p2
    for (int i=0;i<9;i++) if (child[i]==-1) child[i] = p2[i];

    return child;
}

// PMX_Crossover function
SudokuBoard pmx_crossover_per_row(const SudokuBoard &A, const SudokuBoard &B, std::mt19937 &rng) {
    SudokuBoard child = A; // init
    for (int r=0;r<9;r++) {
        // build parent row arrays
        std::array<int,9> p1, p2, c;
        for (int cidx=0;cidx<9;cidx++){ p1[cidx]=A.board[r][cidx]; p2[cidx]=B.board[r][cidx]; }
        c = pmx_row(p1,p2,rng);
        for (int cidx=0;cidx<9;cidx++) child.board[r][cidx] = c[cidx];
    }
    return child;
}

// returns true if swapping values at (row,c1) and (row,c2) does NOT create column/block duplicates
bool isLegalSwap(const SudokuBoard &board, int row, int c1, int c2) {
    int v1 = board.board[row][c1];
    int v2 = board.board[row][c2];

    // check column c1 for v2 excluding position (row,c2)
    for (int r = 0; r < 9; ++r) {
        if (r == row) continue;
        if (board.board[r][c1] == v2) return false;
    }
    // check column c2 for v1 excluding (row,c1)
    for (int r = 0; r < 9; ++r) {
        if (r == row) continue;
        if (board.board[r][c2] == v1) return false;
    }

    // check block for new positions
    auto blockTopLeft = [](int r, int c){ return std::make_pair((r/3)*3,(c/3)*3); };

    //for target (row,c1) with value v2 
    auto t1 = blockTopLeft(row, c1);
    for (int rr = t1.first; rr < t1.first+3; ++rr) {
        for (int cc = t1.second; cc < t1.second+3; ++cc) {
            if (rr == row && cc == c2) continue; // original location of v2
            if (rr == row && cc == c1) continue; // skip itself
            if (board.board[rr][cc] == v2) return false;
        }
    }

    // for target (row,c2) with value v1
    auto t2 = blockTopLeft(row, c2);
    for (int rr = t2.first; rr < t2.first+3; ++rr) {
        for (int cc = t2.second; cc < t2.second+3; ++cc) {
            if (rr == row && cc == c1) continue; // original location of v1
            if (rr == row && cc == c2) continue; // skip itself
            if (board.board[rr][cc] == v1) return false;
        }
    }


    return true;
}

//Cycle_crossover helper function
std::array<int,9> cycleCrossoverRow(const std::array<int,9>& p1,
                                    const std::array<int,9>& p2)
{
    std::array<int,9> child;
    child.fill(-1);

    bool used[9] = { false };

    // Find cycle starting at index 0
    int index = 0;
    while (!used[index]) {
        used[index] = true;
        child[index] = p1[index];

        // find where p1[index] appears in p2
        int nextVal = p2[index];
        index = -1;

        for (int i = 0; i < 9; i++) {
            if (p1[i] == nextVal) {
                index = i;
                break;
            }
        }
        if (index == -1) break;
    }

    // Fill remaining positions from p2
    for (int i = 0; i < 9; i++) {
        if (!used[i]) {
            child[i] = p2[i];
        }
    }

    return child;
}

//Cycle crossover function
SudokuBoard cycleCrossover(const SudokuBoard &A, const SudokuBoard &B)
{
    SudokuBoard child;

    for (int r = 0; r < 9; r++) {
        std::array<int,9> p1, p2;
        for (int c = 0; c < 9; c++) {
            p1[c] = A.board[r][c];
            p2[c] = B.board[r][c];
        }

        auto rowChild = cycleCrossoverRow(p1, p2);

        for (int c = 0; c < 9; c++) {
            child.board[r][c] = rowChild[c];
        }
    }

    return child;
}

std::array<int,9> orderCrossoverRow(const std::array<int,9>& p1,
                                    const std::array<int,9>& p2,
                                    std::mt19937 &rng)
{
    std::array<int,9> child;
    child.fill(-1);

    std::uniform_int_distribution<> cutDist(0, 8);
    int c1 = cutDist(rng);
    int c2 = cutDist(rng);
    if (c1 > c2) std::swap(c1, c2);

    // Copy substring from p1
    for (int i = c1; i <= c2; i++)
        child[i] = p1[i];

    // Fill remaining from p2 in order
    int writePos = (c2 + 1) % 9;
    for (int i = 0; i < 9; i++) {
        int val = p2[i];

        // Skip values already in child
        bool used = false;
        for (int j = c1; j <= c2; j++) {
            if (child[j] == val) {
                used = true;
                break;
            }
        }
        if (used) continue;

        // Write into next free position
        while (child[writePos] != -1)
            writePos = (writePos + 1) % 9;

        child[writePos] = val;
    }

    return child;
}

SudokuBoard orderCrossoverPerRow(const SudokuBoard &A,
                                 const SudokuBoard &B,
                                 std::mt19937 &rng) 
{
    SudokuBoard child = A;
    for (int r = 0; r < 9; r++) {
        std::array<int,9> p1, p2, c;
        for (int col = 0; col < 9; col++) {
            p1[col] = A.board[r][col];
            p2[col] = B.board[r][col];
        }
        c = orderCrossoverRow(p1, p2, rng);
        for (int col = 0; col < 9; col++)
            child.board[r][col] = c[col];
    }
    return child;
}




// mutation function iterates through each row of the sudoku board, it checks the givens vector to make sure
// it is not switching given values, randomly picks nongivens to be swapped and swaps them keeping row consistency
// and checking if it can legally swap, (not creating a duplicate in new column)
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
            int id1 = dist(rng);
            int id2 = dist(rng);
            if (id1 != id2) { 
            int c1 = mutableCols[id1];
            int c2 = mutableCols[id2];
            if (isLegalSwap(board, row, c1, c2)) {
                std::swap(board.board[row][c1], board.board[row][c2]);
                }
        }

        }
        else {
            continue;
        }
    }
    
    return board;
}

// returns a vector of givens for the board in the form of coordinates(x,y)
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


// pick index by roulette using weights w[i] (sumW)
int roulettePick(const std::vector<double>& cumWeights, std::mt19937 &rng) {
    std::uniform_real_distribution<> ur(0.0, cumWeights.back());
    double r = ur(rng);
    auto it = std::lower_bound(cumWeights.begin(), cumWeights.end(), r);
    return int(it - cumWeights.begin());
}


int main() {
    int num_of_samples = 1000; // number of samples per generation
    int num_of_generations = 100; // future use for how many times we want to run generations
    SudokuBoard generation[num_of_samples]; // size of generation can change based on our input
    double mutate_percentage = .6; // determines how much of the time we mutate a board 60% as per paper 9
    double crossover_percentage = .8; // percentage the we actually cross over 2 boards, 80% as per paper 9
    std::mt19937 rng(12345); // random seed
    std::vector<Coordinate> coordinates; // vector array for the givens in our board to solve
    SudokuBoard board_to_solve,board; // board to be solved and temp board variable to assign a generation of boards
    double average_time = 0; 
    double average_solve = 0;
    int boards_solved = 0;
    int num_of_boards = 20;
    int num_trials_per_board = 5;
    clock_t start, end;
    

    std::uniform_real_distribution<> rnd01(0.0, 1.0); // provides randomness used to determine whether to cross or mutate

    for (int m=1;m<=num_of_boards;m++) {
    
        board_to_solve = getRandom17Board();
        cout << "stats for board #" << m << ":\n";

        for (int n=1;n<=num_trials_per_board;n++) {



            // board generation for the very first generation
            for (int i=0;i < num_of_samples;i++) {
                board = randomFillRows(board_to_solve);
                generation[i] = board;
            }

            // calls givens to get the vector of givens for given board
            coordinates = givens(board_to_solve);

            const std::clock_t c_start = std::clock();
            start = clock();

            // running number of generations
            for (int i=0;i<num_of_generations;i++) {
                // assigning fitness and sorting
                for (int i=0;i < num_of_samples;i++) {
                    generation[i].checkValid();
                }
                mergeSort(generation, 0, num_of_samples - 1);

                // weights = 1/(1+errors)
                std::vector<double> weights(num_of_samples);
                double sumW = 0.0;
                for (int i=0;i<num_of_samples;i++){
                    weights[i] = 1.0 / (1.0 + generation[i].fitness);
                    sumW += weights[i];
                }
                std::vector<double> cumW(num_of_samples);
                double acc = 0;
                for (int i=0;i<num_of_samples;i++){
                    acc += weights[i];
                    cumW[i] = acc;
                }


                // crossover, keeping first half of original generation while crossing the 
                // third 25% with the final 25%
                int half_samples = num_of_samples/2;
                int quarter_samples = num_of_samples/4;
                int randnum1 = 0;
                int randnum2 = 0;
                SudokuBoard temp1,temp2;
                for (int i=quarter_samples;i<num_of_samples;i++) {
                    double r = rnd01(rng);
                    if (r < crossover_percentage) {
                        randnum1 = roulettePick(cumW, rng);
                        randnum2 = roulettePick(cumW, rng);
                        temp1 = generation[randnum1];
                        temp2 = generation[randnum2];
                        generation[i] = pmx_crossover_per_row(temp1,temp2,rng); // each crossover type according to paper 8
                        //generation[i] = cycleCrossover(temp1,temp2);
                        //generation[i] = orderCrossoverPerRow(temp1, temp2, rng);


                    }
                }


                for (int i=0;i < num_of_samples;i++) {
                    generation[i].checkValid();
                }
                mergeSort(generation, 0, num_of_samples - 1);

                // mutation function takes all the samples and mutates them according to a percentage
                // we can give and edit the value of
                for (int i=quarter_samples;i<num_of_samples;i++) {
                generation[i] = mutate(generation[i],coordinates,rng,mutate_percentage);
                }
            } 
      

        if (generation[0].fitness == 0) {
            cout << "Board solved!\n";
            generation[0].print();
            boards_solved++;
        }

        end = clock();
        double time_taken = double(end - start);
        cout << "Attempt #" << n << "\n";
        cout << "Best fitness: " << generation[0].fitness << "\n";
        cout << "Time(CPU ticks): " << time_taken << "\n";
        cout << "--------------------------------------\n";



    }

    cout << "--------------------------------------\n";
    cout << "--------------------------------------\n";
    
    }
}