#ifndef SAMPLE_WOR_H
#define SAMPLE_WOR_H

#include <stdlib.h>

#include "gw_knuth.h"


typedef struct {
    
    TreeNode* root;
    TreeNode** nodes;
    TreeNode** left_par;
    //int** nodes_idx;
    double* probs;
    double Q;
    int N;
    int size;
    int node_idx;
    int sampled;

} SamplerState;

SamplerState* get_sampler (double* probs, int N);

SamplerState* get_sampler_v2(double* probs, int N, 
        TreeNode** nodes, TreeNode** left_par);

void destroy_sampler(SamplerState* state);

void destroy_sampler_v2(SamplerState* state);

void restart_sampler(SamplerState* state);

double sum_probs(SamplerState* state);

int sample_wor(SamplerState* state);

#endif //SAMPLE_WOR_H
