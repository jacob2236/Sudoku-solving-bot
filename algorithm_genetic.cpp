#include "algorithm_genetic.hpp"
#include <ctime>
#include <iostream>

Coordinate::Coordinate(int r, int c, int num) : row(r), col(c), num(num) {}


void merge(AbstractBoard gen[], int left, 
                     int mid, int right){
                         
    int index1 = mid - left + 1;
    int index2 = right - mid;

    // Create temp vectors
    AbstractBoard leftVect[index1], rightVect[index2];

    // Copy data to temp vectors left[] and right[]
    for (int i = 0; i < index1; i++)
        leftVect[i] = gen[left + i];
    for (int j = 0; j < index2; j++)
        rightVect[j] = gen[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    // Merge the temp vectors back 
    // into arr[left..right]
    while (i < index1 && j < index2) {
        if (leftVect[i].fitnessValue <= rightVect[j].fitnessValue) {
            gen[k] = leftVect[i];
            i++;
        }
        else {
            gen[k] = rightVect[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of left[], 
    // if there are any
    while (i < index1) {
        gen[k] = leftVect[i];
        i++;
        k++;
    }

    // Copy the remaing elements of right[], 
    // if there are any
    while (j < index2) {
        gen[k] = rightVect[j];
        j++;
        k++;
    }
}

// classic mergesort sorting based on fitness of each board
void mergeSort(AbstractBoard gen[], int left, int right){
    
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(gen, left, mid);
    mergeSort(gen, mid + 1, right);
    merge(gen, left, mid, right);
}

int roulettePick(const std::vector<double>& cumWeights) {
    double choice = (rand() % 10000) / 10000.0; // random %
    choice = choice * cumWeights.back(); // random val between 0 and the max weight
    auto it = std::lower_bound(cumWeights.begin(), cumWeights.end(), choice);
    return int(it - cumWeights.begin());
}

AbstractBoard geneticAlgorithm(AbstractBoard inputBoard, double mutationChance, double crossoverChance){
    int num_of_samples = 1000; // number of samples per generation
    int num_of_generations = 100; // future use for how many times we want to run generations
    AbstractBoard generation[num_of_samples]; // size of generation can change based on our input

    double mutate_percentage = mutationChance; // determines how much of the time we mutate a board 60% as per paper 9
    double crossover_percentage = crossoverChance; // percentage the we actually cross over 2 boards, 80% as per paper 9

    std::vector<Coordinate> coordinates; // vector array for the givens in our board to solve
    SudokuBoard board_to_solve,board; // board to be solved and temp board variable to assign a generation of boards
    
    


       
    // board generation for the very first generation
    for (int i=0;i < num_of_samples;i++) {    
        generation[i] = inputBoard.sample();
    }
        

        // running number of generations
        for (int i=0;i<num_of_generations;i++) {
            // assigning fitness and sorting
            
            for (int i=0;i < num_of_samples;i++) {
                generation[i].getFitness();
                
            }
            
            mergeSort(generation, 0, num_of_samples - 1);
            

            // weights = 1/(1+errors)
            
            std::vector<double> weights(num_of_samples);
            double sumW = 0.0;
            for (int i = 0; i < num_of_samples; i++){
                weights[i] = 1.0 / (1.0 + generation[i].fitnessValue);
                sumW += weights[i];
            }
            std::vector<double> cumW(num_of_samples);
            double acc = 0;
            for (int i = 0; i < num_of_samples; i++){
                acc += weights[i];
                cumW[i] = acc;
            }
            
            // crossover, keeping first half of original generation while crossing the 
            // third 25% with the final 25%
            int half_samples = num_of_samples/2;
            int quarter_samples = num_of_samples/4;
            int randnum1 = 0;
            int randnum2 = 0;
            AbstractBoard temp1,temp2;
            for (int i=quarter_samples;i<num_of_samples;i++) {
                double random = (rand() % 1000) / 1000.0;
                if (random < crossover_percentage) {
                    randnum1 = roulettePick(cumW);
                    randnum2 = roulettePick(cumW);
                    temp1 = generation[randnum1];
                    temp2 = generation[randnum2];
                    generation[i] = temp1.crossover(temp2); // each crossover type according to paper 8
                    //generation[i] = cycleCrossover(temp1,temp2);
                    //generation[i] = orderCrossoverPerRow(temp1, temp2, rng);


                }
            }
            
            for (int i=0;i < num_of_samples;i++) {
                generation[i].getFitness();
            }
            mergeSort(generation, 0, num_of_samples - 1);
            
            // mutation function takes all the samples and mutates them according to a percentage
            // we can give and edit the value of
            for (int i=quarter_samples;i<num_of_samples;i++) {
                double random = (rand() % 1000) / 1000.0;
                if(random < mutate_percentage) {
                    generation[i] = generation[i].sample();
                }
            }
            
        } 
    

    if (generation[0].fitness == 0) {
        cout << "Board solved! trial";
        return generation[0];
    }

    



    return generation[0];



}