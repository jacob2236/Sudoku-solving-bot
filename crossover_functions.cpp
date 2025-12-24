#include "class_sudoku.cpp"

// filler for algos that don't crossover
SudokuBoard dummy (const SudokuBoard &first, const SudokuBoard &second) {
    return first;
}

std::array<int,9> permutationRow(const std::array<int,9>& parent1, const std::array<int,9>& parent2) {
    std::array<int,9> child;
    child.fill(-1);
    int col1 = rand() % 9;
    int col2 = rand() % 9;
    if (col1 > col2) std::swap(col1, col2);
    // copy segment from parent1
    for (int i = col1; i <= col2; i++) {
        child[i] = parent1[i];
    }
    // child contains a random span with digits from parent 1

    // mapping from parent2 segment into child
    for (int i = col1; i <= col2; i++) { // the same span that has already been modified?
        int val = parent2[i]; 

        // if val not already in child segment, place it
        bool present = false;
        for (int j = col1; j <= col2; j++) {
            if (child[j] == val) {
                present = true;
            }
        }
        if (!present) {
            int pos = i;
            int mapped = val;
            // resolve position by following mapping parent2[pos] -> parent1[pos]
            while (true) {
                int v = parent1[pos];
                // find index of v in parent2
                int index = -1;
                for (int k = 0; k < 9; k++) {
                    if (parent2[k] == v) { 
                        index = k; break; 
                    }
                }
                pos = index;
                if (pos < col1 || pos > col2){
                    break;
                } 
            }
            // pos is a free position
            child[pos] = val;
        }
    }

    // fill remaining positions with parent2
    for (int i = 0; i < 9; i++) {
        if (child[i] == -1) {
            child[i] = parent2[i];
        } 
    }

    return child;
}

SudokuBoard permutationRowCrossover(const SudokuBoard &first, const SudokuBoard &second) {
    SudokuBoard result = first; // init
    for (int row=0; row<9; row++) {
        // build parent row arrays
        std::array<int,9> parent1, parent2, child;
        // deep copies of the repective row from each parent despite the const references?
        for (int col=0; col<9; col++){ 
            parent1[col]=first.board[row][col]; 
            parent2[col]=second.board[row][col]; 
        }
        // create a new row by permutating the parent rows
        child = permutationRow(parent1, parent2);
        // update the result board
        for (int col=0; col<9; col++) {
            result.board[row][col] = child[col];
        }
    }
    return result;
}


//Cycle_crossover helper function
std::array<int,9> cycleCrossoverRow(const std::array<int,9>& parent1,
                                    const std::array<int,9>& parent2)
{
    std::array<int,9> child;
    child.fill(-1);

    bool used[9] = { false };

    // Find cycle starting at index 0
    int index = 0;
    while (!used[index]) {
        used[index] = true;
        child[index] = parent1[index];

        // find where parent1[index] appears in parent2
        int nextVal = parent2[index];
        index = -1;

        for (int i = 0; i < 9; i++) {
            if (parent1[i] == nextVal) {
                index = i;
                break;
            }
        }
        if (index == -1) break;
    }

    // Fill remaining positions from parent2
    for (int i = 0; i < 9; i++) {
        if (!used[i]) {
            child[i] = parent2[i];
        }
    }

    return child;
}

//Cycle crossover function
SudokuBoard cycleCrossover(const SudokuBoard &parent1, const SudokuBoard &parent2)
{
    SudokuBoard child;

    for (int row = 0; row < 9; row++) {
        std::array<int,9> parentRow1, parentRow2;
        for (int col = 0; col < 9; col++) {
            parentRow1[col] = parent1.board[row][col];
            parentRow2[col] = parent2.board[row][col];
        }

        auto rowChild = cycleCrossoverRow(parentRow1, parentRow2);

        for (int col = 0; col < 9; col++) {
            child.board[row][col] = rowChild[col];
        }
    }

    return child;
}


std::array<int,9> orderCrossoverRow(const std::array<int,9>& parent1,
                                    const std::array<int,9>& parent2)
{
    std::array<int,9> child;
    child.fill(-1);

    int col1 = rand() % 9;
    int col2 = rand() % 9;
    if (col1 > col2) std::swap(col1, col2);

    // Copy substring from parent1
    for (int i = col1; i <= col2; i++)
        child[i] = parent1[i];

    // Fill remaining from parent2 in order
    int writePos = (col2 + 1) % 9;
    for (int i = 0; i < 9; i++) {
        int val = parent2[i];

        // Skip values already in child
        bool used = false;
        for (int j = col1; j <= col2; j++) {
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

SudokuBoard orderCrossoverPerRow(const SudokuBoard &parent1,
                                 const SudokuBoard &parent2) 
{
    SudokuBoard child = parent1;
    for (int row = 0; row < 9; row++) {
        std::array<int,9> parentRow1, parentRow2, c;
        for (int col = 0; col < 9; col++) {
            parentRow1[col] = parent1.board[row][col];
            parentRow2[col] = parent2.board[row][col];
        }
        c = orderCrossoverRow(parentRow1, parentRow2);
        for (int col = 0; col < 9; col++)
            child.board[row][col] = c[col];
    }
    return child;
}


// crossover by taking a randum upper slice of parent1 and the rest from parent2
SudokuBoard rowWiseCrossover(const SudokuBoard &parent1, const SudokuBoard &parent2) {
    int rowBreak = rand() % 9;
    SudokuBoard child = SudokuBoard();
    for (int row = 0; row < 9; row++) {
        if (row > rowBreak) {
            child.board[row] = parent2.board[row];
        } else {
            child.board[row] = parent1.board[row];
        }
    }
    child.givens = parent1.givens;
    return child;
}

// create a new board by randomly choosing which parent to copy from for each row
SudokuBoard rowRandomCrossover(const SudokuBoard &parent1, const SudokuBoard &parent2) {
    SudokuBoard child = SudokuBoard();
    for (int row = 0; row < 9; row++) {
        int choice = rand() % 2;
        if (choice == 0) {
            child.board[row] = parent1.board[row];
        } else {
            child.board[row] = parent2.board[row];
        }
    }
    child.givens = parent1.givens;
    return child;
}