#ifndef SAMPLE_WOR_H
#define SAMPLE_WOR_H

#include "gw_knuth.h"


typedef struct {
    
    TreeNode* root;
    TreeNode** nodes;
    TreeNode** left_par;
    //int** nodes_idx;
    double* probs;
    int N;
    int node_idx;

} SamplerState;

SamplerState* get_sampler (double* probs, int N);

void destroy_sampler(SamplerState* state);

void restart_sampler(SamplerState* state);

double sum_probs(TreeNode* node);

int sample_wor(SamplerState* state);

#endif //SAMPLE_WOR_H