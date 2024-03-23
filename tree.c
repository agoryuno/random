#include <stdio.h>
#include "tree.h"


void init_tree(int* vals, double* probs, int size) {
    int n = size - 1;
    TreeNode* X[2*size+1];
    TreeNode* P[size+1];

    for (int k = 0; k <= 2*size; k++) {
        X[k] = (TreeNode*)malloc(sizeof(TreeNode));
        if (k <= n) {
            X[k]->WT = probs[k];
            X[k]->L = NULL;
            X[k]->R = NULL;
        }
    }

    P[0] = X[2*size];
    P[0]->WT = INFINITY;
    P[1] = X[0];

    int t = 1;
    int m = n;

    for (int j = 1; j <= n; j++) {
        if (P[t-1]->WT > probs[j]) {
            t++;
            P[t] = X[j];
            break;
        }
        sub_c(X, P, &t, &m, t);
    }

    while (t > 1) {
        sub_c(X, P, &t, &m, t);
    }

}


void mark(TreeNode* P_n, int l) {
    // page 713 (725)
    P_n->level = l;

    if (P_n->L != NULL) {
        mark(P_n->L, l+1);
    } 

    if (P_n->R != NULL) {
        mark(P_n->R, l+1);
    }
}

void build(TreeNode** X, int l, int* m, int* t) {
    // page 713 (725)
    int j = *m;

    if (X[*t]->level == l) {
        X[j]->level = 
    }

}


void sub_c(TreeNode** X, TreeNode** P, int* t, int* m, int k) {
    int j;
    (*m)++;

    X[*m]->L = P[k-1];
    X[*m]->R = P[k];
    X[*m]->WT = P[k-1]->WT + P[k]->WT;

    (*t)--;

    for (j = k; j <= *t; j++) {
        P[j+1] = P[j];
    }

    j = k - 2;
    while (P[j]->WT < X[*m]->WT) {
        P[j+1] = P[j];
        j--;
    }
    P[j+1] = X[*m];
    
    if ( (j>0) && (P[j-1]->WT <= X[*m]->WT) ) {
        sub_c(X, P, t, m, j);
    }
}


// Function to create a new node
TreeNode* create_node(int val, double prob) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }


    new_node->level = 0;
    new_node->key = val;
    new_node->WT = prob;
    new_node->cumm_prob = prob;
    new_node->L = NULL;
    new_node->R = NULL;
    return new_node;
}


// Function to deallocate memory allocated for the tree nodes
void destroy_tree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    destroy_tree(root->L);
    destroy_tree(root->R);
    free(root);
}


