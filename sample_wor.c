#ifdef DEBUG
#include <stdio.h>
#endif

#include "sample_wor.h"



// initializes a sampler and returns a pointer
// to a struct containing the sampler's state
// `probs` is a pointer to an array of probabilities
//   that sum to 1 and represent sampling weights
// `n` is the total number of integers in the sampled
//   range (this should be equal to the length of the `probs` array)
SamplerState* get_sampler (double* probs, int N) {
    SamplerState* state = (SamplerState*)malloc(sizeof(SamplerState));
    state->nodes = (TreeNode**)malloc(sizeof(TreeNode)*(N*2-1));
    state->left_par = (TreeNode**)malloc(sizeof(TreeNode)*(N*2-1));
    //state->nodes_idx = (int*)malloc(sizeof(int)*(N*2-1));
    TreeNode* root = init_tree(probs, N, &state->nodes);

    #ifdef DEBUG
    printf( "get_sampler(): tree initialized\n");
    #endif

    state->root = root;
    state->probs = probs;
    state->N = N;
    state->node_idx = 0;

    #ifdef DEBUG
    printf("get_sampler(): state initialized, root label = %d\n", state->root->label);
    #endif
    return state;
}


// call this when you are done with the sampler
// to free its memory
void destroy_sampler(SamplerState* state) {
    destroy_tree(state->root);
    free(state->nodes);
    free(state->left_par);
    free(state);
}

// restarts the sampler by restoring its initial state 
// without rebuilding the tree - a faster way to get 
// a fresh sampler when sampling probabilities don't change
void restart_sampler(SamplerState* state) {

}

// returns a number between 0 and n-1
int sample_wor(SamplerState* state) {

    #ifdef DEBUG
    printf("entered sample_wor()\n");
    #endif

    TreeNode* node = state->root;
    //TreeNode* left_par[ (state->N*2)-1 ];

    double rnum = (double)rand() / RAND_MAX;  // Generate a random number between 0 and 1
    //double rnum = rand();
    double C = 0.0;

    #ifdef DEBUG
    printf("sample_wor(): rnum=%f, C=%f", rnum, C);
    #endif

    int i = 0;
    while ( (node->LLINK != NULL) || (node->RLINK != NULL) ) {
        i++;
        #ifdef DEBUG
        printf("while %d: C = %f:\n", i, C);
        #endif

        if (node->LLINK != NULL) {

            #ifdef DEBUG
            printf("  llink present\n");
            #endif
            
            if ( rnum < node->G + C )  {

                #ifdef DEBUG
                printf("  rnum < G + C, llink label = %d \n", node->LLINK->label);
                #endif

                
                state->left_par[state->node_idx] = node;
                state->node_idx++;
                node = node->LLINK;
                continue;
            } else {

                #ifdef DEBUG
                printf("  rnum >= G + C\n");
                #endif

                if (node->RLINK == NULL) {

                    #ifdef DEBUG
                    printf("  no rlink\n");
                    #endif

                    break;
                } else {
                    #ifdef DEBUG
                    printf("  rlink present\n");
                    #endif

                    C += node->G;
                    node = node->RLINK;
                }
            }
        } else {
            #ifdef DEBUG
            printf("no llink\n");
            #endif
        }
        
        
    }

    #ifdef DEBUG
    printf("out of while loop: node_idx = %d, node label = %d\n", 
        state->node_idx,
        node->label);
    #endif

    for (int j = 0; j < state->node_idx; j++) {
        #ifdef DEBUG
        printf("in for loop, j = %d: \n", j);
        printf("  state->left_par[j]->label = %d\n", state->left_par[j]->label);

        #endif

        state->left_par[j]->G -= node->WT;
    }

    #ifdef DEBUG
    printf("\n");
    #endif

    return node->label;
}