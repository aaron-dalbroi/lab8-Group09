#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

// Prints out an int array. Used for diagnostics.
void printOutArray(int* ints, int size){
    for(int i = 0; i < size; i++){
        printf("%d ", ints[i]);
    }
    printf("\n");
}

// Parse a vector of integers from a file, one per line.
// Return the number of integers parsed.
int parseInts(FILE *file, int **ints) {
    
    if (file == NULL) {
        return -1; 
    }

    int *int_array = NULL;
    int array_size = 0;    

    // Read integers from the file, one per line
    int num;
    while (fscanf(file, "%d\n", &num) == 1) {
        // Allocate memory for the new integer in the array
        int *temp = (int *)realloc(int_array, (array_size + 1) * sizeof(int));
        if (temp == NULL) {
            // Memory allocation failed, clean up and return -1
            free(int_array);
            return -1;
        }
        int_array = temp;
        int_array[array_size] = num;
        array_size++;
    }

    // Check for errors during file reading
    if (!feof(file)) {
        // An error occurred during file reading, clean up and return -1
        free(int_array);
        return -1;
    }

    // Assign the dynamically allocated array to the output pointer
    *ints = int_array;

    // Return the number of integers parsed
    return array_size;
}


// Write a vector of integers to a file, one per line.
void writeInts(FILE *file, int *ints, int size) {
    // Check if the file is valid
    if (file == NULL) {
        fprintf(stderr, "Invalid file pointer\n");
        return;
    }

    // Write each integer to the file, one per line
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", ints[i]);
    }
}


// Returns the result of a sequential prefix scan of the given vector of integers.
int* SEQ(int *ints, int size) {
    
    // Initialize new vector.
    int* newVector = (int*)malloc(sizeof(int)*size);

    newVector[0] = ints[0];

    // Each output vector index is the previous index plus the next number in the input.
    for(int i = 1; i < size; i++){
        newVector[i] = newVector[i-1] + ints[i];
        
    }
    
    return newVector;
}

// Performs a single Hillis/Steele step.
void hillisSteeleStep(int* ints, int size, int stepNum){
    int stepSize = round(pow(2, stepNum));

    // printOutArray(ints, size);

    for(int i = size - 1; i >= stepSize; i--){
        ints[i] = ints[i] + ints[i - stepSize];
    }

    // printOutArray(ints, size);
    // printf("\n");
}

// Return the result of Hillis/Steele, but with each pass executed sequentially.
int* HSS(int *ints, int size) {
    // Allocate memory for the result array.
    int * resultArray = malloc(sizeof(int) * size);
    // Copy ints into result array.
    for(int i = 0; i < size; i++){
        resultArray[i] = ints[i];
    }

    // Calculate the number of Hillis/Steele steps to perform.
    int numOps = round(log(size) / log(2));

    // Perform each step.
    for(int i = 0; i < numOps; i++){
        hillisSteeleStep(resultArray, size, i);
    }

    return resultArray;
}


// Return the result of Hillis/Steele, parallelized using pthread
int* HSP(int *ints, int size, int numthreads) {
    int * newVector = (int*)malloc(sizeof(int)*size);

    // TODO: Implement.



    return newVector;
}


int main(int argc, char** argv) {

    if ( argc != 5 ) {
        printf("Usage: %s <mode> <#threads> <input file> <output file>\n", argv[0]);
        return 1;
    }

    char *mode = argv[1]; // SEQ or HSS (HS Sequential) or HSP (HS Parallel).
    int numThreads = atoi(argv[2]); // Only used in HSP.
    FILE *input = fopen(argv[3], "r");
    FILE *output = fopen(argv[4], "w");

    int *ints;
    int size;
    size = parseInts(input, &ints);

    int *result;
    if (strcmp(mode, "SEQ") == 0) {
        result = SEQ(ints, size);
    } else if (strcmp(mode, "HSS") == 0) {
        result = HSS(ints, size);
    } else if (strcmp(mode, "HSP") == 0) {
        result = HSP(ints, size, numThreads);
    } else {
        printf("Unknown mode: %s\n", mode);
        return 1;
    }

    writeInts(output, result, size);
    fclose(input);
    fclose(output);
    return 0;
}
