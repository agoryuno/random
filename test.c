#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <time.h>
#include <assert.h>

//#include "tree.h"
//#include "gw_knuth.h"
#include "sample_wor.h"

int main() {
    //srand(time(NULL));
    //double random_number = (double)rand() / RAND_MAX;  // Generate a random number between 0 and 1

    //int vals[] = {0,1,2,3,4,5,6,7,8,9};
    double probs[10] = {0.1, 0.05, 0.05, 0.1, 0.2, 0.08, 0.02, 0.1, 0.15, 0.15};

    double sum_prob = 0.0;
    for (int i=0; i<10; i++) {
        sum_prob += probs[i];
    }
    assert(sum_prob == 1.0);

    SamplerState* sampler = get_sampler(probs, 10);

    #ifdef DEBUG
    printf("main(): received sampler, root label = %d\n", sampler->root->label);
    #endif
    printf("sampled: %d\n", sample_wor(sampler));
    //TreeNode* root = init_tree(probs, 10);
    //printf("root label = %d\n", root->label);
    //print_tree(root);
    
    //#ifdef DEBUG
    //printf("PROGRAM FINISHED\n");
    //print_tree(root, 0);
    //#endif

    //destroy_tree(root);
    return 0;
}