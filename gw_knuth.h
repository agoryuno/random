#ifndef GW_KNUTH_H 1
#define GW_KNUTH_H 1

#include <stdlib.h>
#include <math.h>

#include <stdio.h>

typedef struct tree_node {
    int label;
    double WT;
    double G;
    double H;
    struct tree_node* LLINK;
    struct tree_node* RLINK;
    struct tree_node* parent;
} TreeNode;

TreeNode* init_tree(double* w, int N, TreeNode*** nodes);

void print_tree(TreeNode* node, int depth);

void destroy_tree(TreeNode* root);

#endif // GW_KNUTH_H