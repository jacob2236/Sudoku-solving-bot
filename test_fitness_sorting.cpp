#include "class_sudoku.cpp"
#include "board_loader.cpp"
#include "brute_force.cpp"
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
        if (L[i].fitness >= R[j].fitness) {
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


int main() {

    int num_of_generations = 5; // future use for how many times we want to run generations
    SudokuBoard generation[3]; // size of generation can change based on our input

    // Creates boards and then uses check valid function to assign fitness. 
    // Right now checkValid will not work after calling getrandboard,
    // can only call checkvalid after calling bruteforce algo, but
    // gets stuck after 3 bruteforce calls
    for (int i=0;i < sizeof(generation)/sizeof(generation[0]);i++) {

        SudokuBoard board = getRandomBoard();
        SudokuBoard solvedBoard = bruteForce(board);
        solvedBoard.checkValid();
        generation[i] = solvedBoard;
    }
    cout << "boards generated...\n";
    int n = sizeof(generation)/sizeof(generation[0]);
    
    // sorts boards based on fitness of entire board using merge sort
    mergeSort(generation, 0, n - 1);

    // used for testing arangement of array
    for (int i=0; i<sizeof(generation)/sizeof(generation[0]);i++) {
        cout << generation[i].fitness << "\n";
    }
}