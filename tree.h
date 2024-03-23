#ifndef TREE_H 1
#define TREE_H 1

#include <stdlib.h>
#include <math.h>

typedef struct tree_node {
    int key;
    int level;
    double WT;
    double cumm_prob;
    struct tree_node* L;
    struct tree_node* R;
} TreeNode;

TreeNode* create_node(int val, double prob);

void init_tree(int* vals, double* probs, int K);

void sub_c(TreeNode** X, TreeNode** P, int* t, int* m, int k);

void mark(TreeNode* P_n, int l);

void build(int l, int* m, int* t);

#endif