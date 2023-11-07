#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Parse a vector of integers from a file, one per line.
// Return the number of integers parsed.
int parse_ints(FILE *file, int **ints) {
    
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
void write_ints(FILE *file, int *ints, int size) {
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


// Return the result of a sequential prefix scan of the given vector of integers.
int* SEQ(int *ints, int size) {
    
    //Initialize new vector
    int* new_vector = (int*)malloc(sizeof(int)*size);

    new_vector[0] = ints[0];

    //Each output vector index is the previous index plus the next number in the input
    for(int i = 1; i < size; i++){
        new_vector[i] = new_vector[i-1] + ints[i];
        
    }
    //return output vector
    return new_vector;

}


// Return the result of Hillis/Steele, but with each pass executed sequentially
int* HSS(int *ints, int size) {

}


// Return the result of Hillis/Steele, parallelized using pthread
int* HSP(int *ints, int size, int numthreads) {

}


int main(int argc, char** argv) {

    if ( argc != 5 ) {
        printf("Usage: %s <mode> <#threads> <input file> <output file>\n", argv[0]);
        return 1;
    }

    char *mode = argv[1];
    int num_threads = atoi(argv[2]);
    FILE *input = fopen(argv[3], "r");
    FILE *output = fopen(argv[4], "w");

    int *ints;
    int size;
    size = parse_ints(input, &ints);

    int *result;
    if (strcmp(mode, "SEQ") == 0) {
        result = SEQ(ints, size);
    } else if (strcmp(mode, "HSS") == 0) {
        result = HSS(ints, size);
    } else if (strcmp(mode, "HSP") == 0) {
        result = HSP(ints, size, num_threads);
    } else {
        printf("Unknown mode: %s\n", mode);
        return 1;
    }

    write_ints(output, result, size);
    fclose(input);
    fclose(output);
    return 0;
}
