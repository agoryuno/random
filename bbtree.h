#ifndef BBTREE_H 1
#define BBTREE_H 1

#include <stdlib.h>
#include <math.h>


typedef struct tree_node {
    int key;
    int B;
    struct tree_node* LLINK;
    struct tree_node* RLINK;
} TreeNode;

#endif