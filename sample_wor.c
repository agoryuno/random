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


// find all leaves and return the sum of their 
// probabilities
double sum_probs(TreeNode* node) {
    if (node == NULL) {
        return 0.0;
    }

    // Check if the current node is a leaf
    if (node->LLINK == NULL && node->RLINK == NULL) {
        return node->WT;
    }

    // Recursively visit left and right subtrees
    double left_sum = sum_probs(node->LLINK);
    double right_sum = sum_probs(node->RLINK);
    return left_sum + right_sum;
}

// returns a number between 0 and n-1
int sample_wor(SamplerState* state) {

    #ifdef DEBUG
    printf("entered sample_wor()\n");
    #endif

    TreeNode* node = state->root;
    //TreeNode* left_par[ (state->N*2)-1 ];

    // Q is the sum of probabilities of all
    // remaining leaves
    double Q = sum_probs(state->root);
    double rnum = ((double)rand() / RAND_MAX ) * Q; 
    //double rnum = rand();
    double C = 0.0;

    #ifdef DEBUG_PRINT
        printf("sample_wor(): rnum=%f, C=%f\n", rnum, C);
    #endif

    int i = 0;
    while ( (node->LLINK != NULL) || (node->RLINK != NULL) ) {
        i++;
        #ifdef DEBUG_PRINT
            printf("while %d: C = %f:\n", i, C);
        #endif

        if (node->LLINK != NULL) {

            #ifdef DEBUG_PRINT
                printf("  llink present\n");
            #endif
            
            if ( rnum < node->G + C )  {

                #ifdef DEBUG_PRINT
                    printf("  rnum < G + C (%f + %f = %f), llink label = %d \n", 
                        node->G, C, node->G + C, node->LLINK->label);
                #endif

                
                state->left_par[state->node_idx] = node;
                state->node_idx++;
                node = node->LLINK;
                continue;
            } else {

                #ifdef DEBUG_PRINT
                    printf("  rnum >= G + C (%f + %f = %f\n",
                        node->G, C, node->G + C);
                #endif

                if (node->RLINK == NULL) {

                    #ifdef DEBUG_PRINT
                        printf("  no rlink\n");
                    #endif

                    break;
                } else {
                    #ifdef DEBUG_PRINT
                        printf("  rlink present\n");
                    #endif

                    C += node->G;
                    node = node->RLINK;
                }
            }
        } else {
            #ifdef DEBUG_PRINT
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