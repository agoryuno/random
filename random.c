#include <stdlib.h>
#include <time.h>

// Function to randomly sample values from array v with given probabilities p
int* sample_values(
            int* v,   // array of integers to be sampled
            double* p, // sampling probabilities of all integers
            int K,    // length of the `v` and `p` arrays
            int N     // number of integeres to be sampled
            ) {  
    // Allocate memory for the sampled values
    int* sampled_values = (int*)malloc(N * sizeof(int));

    // Calculate cumulative probabilities
    double* cumulative_probs = (double*)malloc(K * sizeof(double));
    cumulative_probs[0] = p[0];
    for (int i = 1; i < K; i++) {
        cumulative_probs[i] = cumulative_probs[i - 1] + p[i];
    }

    // Perform sampling
    for (int i = 0; i < N; i++) {
        // Generate a random number between 0 and 1
        double random_num = rand() / RAND_MAX;

        // Find the index corresponding to the random number
        int j;
        for (j = 0; j < K; j++) {
            if (random_num <= cumulative_probs[j]) {
                sampled_values[i] = v[j];
                break;
            }
        }

        // Adjust cumulative probabilities
        for (int k = j; k < K; k++) {
            cumulative_probs[k] -= p[j];
        }
        K--;
    }

    // Free allocated memory
    free(cumulative_probs);

    return sampled_values;
}