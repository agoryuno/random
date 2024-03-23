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
    state->root = root;
    state->probs = probs;
    state->N = N;
    state->node_idx = 0;
    return state;
}


// call this once you are done with the sampler
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
    TreeNode* node = state->root;
    //TreeNode* left_par[ (state->N*2)-1 ];

    double rnum = rand();
    double C = 0.0;

    while ( (node->LLINK != NULL) || (node->RLINK != NULL) ) {
        if (node->LLINK != NULL) {
            if ( rnum < node->G + C )  {
                state->left_par[state->node_idx] = node;
                state->node_idx++;
                node = node->LLINK;
                continue;
            } else {
                if (node->RLINK == NULL) {
                    break;
                } else {
                    C += node->G;
                    node = node->RLINK;
                }
            }
        } 
    }

    for (int j = 0; j <= state->node_idx; j++) {
        state->left_par[j]->G -= node->WT;
    }
    return node->label;
}