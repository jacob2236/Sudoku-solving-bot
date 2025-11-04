#include "class_sudoku_possibilities.cpp"
#include <deque>
#include <array>

/* Tabu Search as described in citation [15].
A local search in which recently searched boards are tracked to prevent repetition.
The paper promises good results so I'm excited to see how it plays out.

Unfortunatly, no source code was provided and not all hyperparameters were discussed (particularly tabu list length).
This will be my best attempt at a faithful recreation based on the psuedocode.
If time energy and sanity permit, I'd like to tinker with this further.

*/

int rand9() {
    return std::rand() % 9;// mild convenience to return a random digit 1-9
}

array<int, 3> getDifference(const PossibilitiesBoard& newBoard, const PossibilitiesBoard& original) {
    //assumes only a single cell difference. "returns" the tabu triplet
    for(int row = 0; row < 9; row++) {
        for(int col = 0; col < 9; col++){
            if(original.board[row][col] != newBoard.board[row][col]) {
                array<int, 3> tabu;
                tabu[0] = row;
                tabu[1] = col;
                tabu[2] = original.board[row][col];
                return tabu;
            }
        }
    }
}

int globalTabuLength;
PossibilitiesBoard tabuSearch(SudokuBoard board, int maxIter, int tabuLength, int candidateLength) {


    PossibilitiesBoard bestBoard = PossibilitiesBoard(board);
    globalTabuLength = tabuLength;
    deque<array<int, 3>> tabuQueue; // use deque for random access to check existing tabus

    for(int iter = 0; iter < maxIter; iter++) {
        PossibilitiesBoard candidateList[candidateLength];
        int candidates = 0;
        while(candidates < candidateLength) {
            PossibilitiesBoard newCandidate = bestBoard.mutate();
            bool isValid = true;
            for(int i = 0; i < tabuQueue.size(); i++){
                int tabu[3] = {tabuQueue[i][0], tabuQueue[i][1], tabuQueue[i][2]};
                if(newCandidate.board[tabu[0]][tabu[1]] == tabu[2]){
                    isValid = false;
                    break;
                }
            }
            if (isValid){
                candidateList[candidates] = newCandidate;
                candidates++;
            }
        }
        PossibilitiesBoard *nextBest;
        int bestFitness = 0;
        for(PossibilitiesBoard candidate : candidateList) {
            if(candidate.fitness() > bestFitness) {
                bestFitness = candidate.fitness();
                nextBest = &candidate;
            }
        }
        if(bestFitness == 81) {
            //only the solution can have a fitness of 81 filled cells
            return *nextBest;
        }
        if(bestFitness > bestBoard.fitness()) {
            array<int, 3> newTabu = getDifference(*nextBest, bestBoard);
            tabuQueue.push_front(newTabu);
            if(tabuQueue.size() > tabuLength) {
                tabuQueue.pop_back();
            }
            bestBoard = *nextBest;
        }       
    }
    return bestBoard; //couldn't find the solution, here's how close we got.
}



