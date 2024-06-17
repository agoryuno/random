#ifdef DEBUG_PRINT
#include <stdio.h>
#endif

#include "sample_wor.h"

static void __destroy_sampler(SamplerState* state, int nodes) {
    destroy_tree(state->root);

    if (nodes == 1) {
        free(state->nodes);
        free(state->left_par);
    }
    free(state);
}

// find all leaves and return the sum of their 
// probabilities
double sum_probs(SamplerState* state) {
    double cumprob = 0.0;
    // the leaves are in the first `state->N`
    // elements of `state->nodes`
    for (int i = 0; i < state->N; i++) {
        cumprob += state->nodes[i]->WT;
    }
    return cumprob;
}

// initializes a sampler and returns a pointer
// to a struct containing the sampler's state.
// Uses `nodes` and `left_par` arrays supplied by
// the caller, who is responsible for allocating and 
// disposing of them, ensuring that their size is greater
// than or equal to `N`*2-1
// `probs` is a pointer to an array of probabilities
//   that sum to 1 and represent sampling weights
// `N` is the total number of integers in the sampled
//   range (this should be equal to the length of the `probs` array)
SamplerState* get_sampler_v2(double* probs, int N, 
        TreeNode** nodes, TreeNode** left_par) {
    if (N<=1) {
        return NULL;
    }
    SamplerState* state = malloc(sizeof(SamplerState));
    state->nodes = nodes;
    state->left_par = left_par;
    TreeNode* root = init_tree(probs, N, &state->nodes);

    state->N = N;
    state->sampled = 0;
    state->Q = sum_probs(state);
    state->root = root;
    state->probs = probs;
    state->N = N;
    state->node_idx = 0;

    return state;
}

// initializes a sampler and returns a pointer
// to a struct containing the sampler's state
// `probs` is a pointer to an array of probabilities
//   that sum to 1 and represent sampling weights
// `n` is the total number of integers in the sampled
//   range (this should be equal to the length of the `probs` array)
SamplerState* get_sampler (double* probs, int N) {
    if (N<=1) {
        return NULL;
    }
    SamplerState* state = malloc(sizeof(SamplerState));
    state->nodes = malloc(sizeof(TreeNode*)*(N*2-1));
    state->left_par = malloc(sizeof(TreeNode*)*(N*2-1));
    TreeNode* root = init_tree(probs, N, &state->nodes);

    state->N = N;
    state->sampled = 0;
    state->Q = sum_probs(state);

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

// Call this to destroy the sampler created with
// get_sampler(). 
void destroy_sampler(SamplerState* state) {
    __destroy_sampler(state, 1);
}

// Call this to destroy the sampler created with
// get_sampler_v2().
void destroy_sampler_v2(SamplerState* state) {
    __destroy_sampler(state, 0);
}

// restarts the sampler by restoring its initial state 
// without rebuilding the tree - a faster way to get 
// a fresh sampler when sampling probabilities don't change
void restart_sampler(SamplerState* state) {

}

// returns a number between 0 and n-1
int sample_wor(SamplerState* state) {

    // guard against sampling more times
    // than there are leaves in the treee
    if (state->sampled >= state->N) {
        #ifdef ABORT_ON_OVERSAMPLE
            fprintf(stderr, "Attempting to sample over N. N = %d, already made %d samples. Aborting!\n",
                state->N, state->sampled);
            exit(EXIT_FAILURE);
        #else
            return NULL;
        #endif
    }

    TreeNode* node = state->root;

    // Q is the sum of probabilities of all
    // remaining leaves
    double Q = state->Q;
    double rnum = ((double)random() / RAND_MAX ) * Q; 
    double C = 0.0;

    #ifdef DEBUG_PRINT
        printf("sample_wor(): rnum=%f, C=%f, Q=%f\n", rnum, C, Q);
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

                if (node->RLINK == NULL) {

                    #ifdef DEBUG_PRINT
                        printf("  no rlink\n");
                    #endif

                    break;
                } else {
                    #ifdef DEBUG_PRINT
                        printf("  rnum >= G + C (%f + %f = %f), rlink = %d\n",
                        node->G, C, node->G + C, node->RLINK->label);
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

    #ifdef DEBUG_PRINT
    printf("out of while loop: node_idx = %d, node label = %d\n", 
        state->node_idx,
        node->label);
    #endif

    #ifdef DEBUG_PRINT
        printf("Decrementing visited links' G:\n");
    #endif
    for (int j = 0; j < state->node_idx; j++) {
        #ifdef DEBUG_PRINT
        printf("    %d: state->left_par[j]->label = %d\n", i, state->left_par[j]->label);

        #endif

        state->left_par[j]->G -= node->WT;
        // reset left_par
        state->left_par[j] = NULL;
    }

    // decrease the value of Q
    state->Q -= node->WT;

    // reset visited left nodes count
    state->node_idx = 0;

    // increment the sampled counter
    state->sampled++;
    #ifdef DEBUG_PRINT
    printf("\n");
    #endif

    return node->label;
}