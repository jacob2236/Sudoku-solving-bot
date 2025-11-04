#include "class_sudoku.cpp"
#include "board_loader.cpp"
#include "brute_force.cpp"
#include <cstdlib>
using namespace std;

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
    for (int i=0;i<5;i++) {
        for (int j=0;j<9;j++) {
            answer.board[i][j] = temp1.board[i][j];

        }
    }
    for (int i=5;i<9;i++) {
        for (int j=0;j<9;j++) {
            answer.board[i][j] = temp2.board[i][j];

        }
    }
    return answer;
}


int main() {
    int num_of_samples = 10;
    int num_of_generations = 5; // future use for how many times we want to run generations
    SudokuBoard generation[num_of_samples]; // size of generation can change based on our input
    srand(6);
    // board generation for the very first generation
    for (int i=0;i < num_of_samples;i++) {
        SudokuBoard board = getRandomBoard();
        generation[i] = board;
    }

    // running number of generations
    for (int n=0;n<num_of_generations;n++) {
        // assigning fitness and sorting
        for (int i=0;i < num_of_samples;i++) {
            generation[i].checkValid();
        }
        mergeSort(generation, 0, num_of_samples - 1);

        // testing each sorted generation
        cout << "This prints the " << (n+1) << "th generation fitness values to test crossovers and generations\n";
        for (int j=0;j<num_of_samples;j++) {
            cout << generation[j].fitness << "\n";
        }
        cout << "------------\n";

        // crossover, keeping first half of original generation while crossing the 
        // third 25% with the final 25%
        int half_samples = num_of_samples/2;
        int quarter_samples = num_of_samples/4;
        int randnum1 = 0;
        int randnum2 = 0;
        SudokuBoard temp1,temp2;
        for (int i=half_samples;i<num_of_samples;i++) {
            randnum1 = rand()%(quarter_samples) + half_samples;
            randnum2 = rand()%(quarter_samples) + (half_samples + quarter_samples);
            temp1 = generation[randnum1];
            temp2 = generation[randnum2];
            generation[i] = cross(temp1,temp2);
        }
    }
}