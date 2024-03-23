#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure to represent a bin in the alias method
typedef struct {
    int index;
    double probability;
    char alias;
} Bin;

// Function to initialize bins with probabilities
void initializeBins(double *probabilities, int num_probabilities, Bin *bins, int num_bins) {

    // Calculate probabilities and initialize bins
    for (int i = 0; i < num_probabilities; i++) {
        bins[i].index = i;
        bins[i].probability = probabilities[i];
        bins[i].alias = '\0';
    }

    // Initialize the rest of the bins
    for (int i = num_probabilities; i < num_bins; i++) {
        bins[i].index = -1;
        bins[i].probability = 0;
        bins[i].alias = '\0';
    }
}

// Function to generate aliases for bins
void generateAliases(Bin *bins, int num_bins) {
    int *small = (int *)malloc(num_bins * sizeof(int));
    int *large = (int *)malloc(num_bins * sizeof(int));
    int *stack = (int *)malloc(num_bins * sizeof(int));

    int num_small = 0, num_large = 0;

    // Partition the bins into small and large
    for (int i = 0; i < num_bins; i++) {
        if (bins[i].probability < 1.0)
            small[num_small++] = i;
        else
            large[num_large++] = i;
    }

    // Generate aliases
    while (num_small > 0 && num_large > 0) {
        int small_index = small[--num_small];
        int large_index = large[--num_large];

        bins[small_index].alias = large_index;
        bins[large_index].probability -= (1.0 - bins[small_index].probability);

        if (bins[large_index].probability < 1.0)
            small[num_small++] = large_index;
        else
            large[num_large++] = large_index;
    }

    free(small);
    free(large);
    free(stack);
}

// Function to sample from the bins without replacement
int sampleWithoutReplacement(Bin *bins, int num_bins, int *used, int *num_used) {
    // Generate a random index
    int index = rand() % num_bins;

    // Use the alias method to choose a bin
    while (used[bins[index].index] && num_used[bins[index].index] >= 1) {
        index = bins[index].alias;
    }

    // Update used and num_used arrays
    used[bins[index].index] = 1;
    num_used[bins[index].index]++;

    return bins[index].index;
}


// Function to calculate the number of bins based on the number of weights
int calculateNumBins(int num_weights) {
    // If num_weights is already a power of 2, return num_weights
    if ((num_weights & (num_weights - 1)) == 0) {
        return num_weights;
    }

    // Find the smallest power of 2 greater than num_weights
    int power_of_2 = 1;
    while (power_of_2 < num_weights) {
        power_of_2 <<= 1;
    }

    return power_of_2;
}


int* sample_values(int* values, double* probs, int K, int N) {
    int num_bins = calculateNumBins(K);
    printf("num_bins = %d\n", num_bins);
    Bin* bins = (Bin*)malloc(sizeof(Bin)*num_bins);
    initializeBins(probs, K, bins, num_bins);
    generateAliases(bins, num_bins);

    // Array to keep track of used items
    int used[K];
    // Array to keep track of the number of times an item has been used
    int num_used[K];
    for (int i = 0; i < K; i++) {
        used[i] = 0;
        num_used[i] = 0;
    }

    int* result = (int*)malloc(sizeof(int)*N);
    // Sample from the bins without replacement
    for (int i = 0; i < N; i++) {
        int sampled_index = sampleWithoutReplacement(bins, num_bins, used, num_used);
        result[i] = sampled_index;
    }
    free(bins);
    return result;
}

int main() {
    int vals[] = {1,2,3,4,5,6,7,8,9,0};
    double probs[] = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};
    int K = 10;
    int N = 4;

    srand (time(NULL));

    int* sample = sample_values(vals, probs, K, N);
    for (int i =0; i<N; i++) {
        printf("sampled: %d\n", sample[i]);
    }
}