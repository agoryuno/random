// source: https://www-cs-faculty.stanford.edu/~knuth/programs/garsia-wachs.w

#include "gw_knuth.h"


static void combine(
        int k,
        int* l,
        int* r,
        int* t,
        double* w,
        int* v,
        double* q,
        int* m) {
    int j,d;
    double x  = q[k-1] + q[k]; 

    (*m)++;

    l[*m] = v[k-1]; 
    r[*m] = v[k]; 
    w[*m] = x;

    #ifdef DEBUG
    printf("combine(): node %d(%f)=%d(%f)+%d(%f)\n", *m, x, l[*m], w[l[*m]], r[*m], w[r[*m]]);
    #endif

    (*t)--;
    
    for (j=k; j<=*t; j++) {
        q[j] = q[j+1]; 
        v[j] = v[j+1];
    }

    for (j=k-2; q[j] < x; j--) {
        q[j+1] = q[j];
        v[j+1] = v[j];
    }

    q[j+1] = x; 
    v[j+1] = *m;
    
    #ifdef DEBUG
    printf("combine(): ");
    for (d=1; d<=*t; d++) printf("q[%d]=%f ", d, q[d]);
    printf("\n");
    #endif

    while ((j>0) && (q[j-1] <= x)) {
        d = *t - j; 
        combine(j, l, r, t, w, v, q, m); 
        j = *t - d;
    }
}


static void mark(
        int k, /* node */
        int p, /* starting depth */
        int* d,
        int* l,
        int* r) {

 
    #ifdef DEBUG
    printf("mark(): node %d has depth %d | l[k]=%d, r[k]=%d\n", k, p, l[k], r[k]);
    #endif

    d[k]=p;
    if (l[k]>=0) mark(l[k],p+1, d, l, r);
    if (r[k]>=0) mark(r[k],p+1, d, l, r);
}


/* The |build| subroutine rebuilds a tree from the depth table,
by doing a depth-first search according a slick idea by Bob Tarjan.
It creates a tree rooted at node~|m| having leftmost leaf~|t|. */
static void build(
        int b, /* depth of node |m|, plus 1 */
        int* d,
        int* l,
        int* r,
        int* t,
        int* m,
        TreeNode*** nodes
    ) {
    int j = *m;

    #ifdef DEBUG
    printf ("build(): j=%d, b=%d, d[t]=%d\n", j, b, d[*t]);
    
    #endif

    if (d[*t]==b) {
        l[j] = *t;
        (*nodes)[j]->LLINK = (*nodes)[*t];
        (*nodes)[j]->LLINK->parent = (*nodes)[j];
        (*t)++;
    } else {
        (*m)--; 
        l[j] = *m;
        (*nodes)[j]->LLINK = (*nodes)[*m];
        (*nodes)[j]->LLINK->parent = (*nodes)[j];
        build(b+1, d, l, r, t, m, nodes);
    }
    if (d[*t]==b) {
        r[j] = *t;
        (*nodes)[j]->RLINK = (*nodes)[*t];
        (*nodes)[j]->RLINK->parent = (*nodes)[j];
        (*t)++;
    } else {
        (*m)--; 
        r[j] = *m; 
        (*nodes)[j]->RLINK = (*nodes)[*m];
        (*nodes)[j]->RLINK->parent = (*nodes)[j];
        build(b+1, d, l, r, t, m, nodes);
    }
    #ifdef DEBUG_PRINT
    printf(" node %d = %d + %d\n", j, l[j], r[j]);
    printf("llink %d prob=%f\n", (*nodes)[j]->LLINK->label, (*nodes)[j]->LLINK->WT);
    printf("rlink %d prob=%f\n", (*nodes)[j]->RLINK->label, (*nodes)[j]->RLINK->WT);
    #endif
}


static void bottom_up(TreeNode* node) {
    if (node->LLINK != NULL) {
        // if node has a leaf as left-descendant
        if ( (node->LLINK->LLINK == NULL) && (node->LLINK->RLINK == NULL) ) {
            node->G = node->LLINK->WT;
        } else {
            node->G = node->LLINK->G + node->LLINK->H;
        }
    }

    if (node->RLINK != NULL) {
        // if node has a leaf as right-descendant
        if ( (node->RLINK->LLINK == NULL) && (node->RLINK->RLINK == NULL) ) {
            node->H = node->RLINK->WT;
        } else {
            node->H = node->RLINK->G + node->RLINK->H;
        }
    }

    if (node->parent != NULL) {
        bottom_up(node->parent);
    }
}

static inline void assign_probs(TreeNode*** nodes, int n) {
    for (int i = 0; i <= n; i++) {
        bottom_up((*nodes)[i]->parent);
    }
}

TreeNode* init_tree(double* probs, int N, TreeNode*** nodes) {

    int size = N*2 + 1;
    int n = N-1;

    double w[size]; // full array of probabilities;
    int l[size], r[size]; /* left and right children */
    int d[size]; /* depth */
    double q[size]; /* working region */
    int v[size]; /* number of node in working region */
    int t; /* current size of working region */
    int m; /* current node */

    m = n;
    t = 1;

    #ifdef DEBUG_PRINT
        printf("init_tree(): N=%d, n=%d, size=%d\n", N, n, size);
    #endif

    for (int j = 0; j <= m; j++) {
        l[j] = -1;
        r[j] = -1;
        w[j] = probs[j];

        (*nodes)[j] = malloc(sizeof(TreeNode));
        (*nodes)[j]->LLINK = NULL;
        (*nodes)[j]->RLINK = NULL;
        (*nodes)[j]->WT = probs[j];
        (*nodes)[j]->label = j;
        (*nodes)[j]->parent = NULL;
        #ifdef DEBUG_PRINT
            printf("created node %d, label=%d, prob=%f\n", j, (*nodes)[j]->label, (*nodes)[j]->WT);
        #endif
    }

    for (int j = m+1; j <= 2*n; j++ ) {
        (*nodes)[j] = malloc(sizeof(TreeNode));
        (*nodes)[j]->LLINK = NULL;
        (*nodes)[j]->RLINK = NULL;
        (*nodes)[j]->WT = INFINITY;
        (*nodes)[j]->label = j;
        (*nodes)[j]->parent = NULL;
        #ifdef DEBUG_PRINT
            printf("created node %d, label=%d, prob=%f\n", j, (*nodes)[j]->label, (*nodes)[j]->WT);
        #endif
    }

    q[0] = INFINITY;
    q[1] = w[0];
    v[1] = 0;

    for (int k = 1; k <= n; k++) {

        #ifdef DEBUG_PRINT
            printf ("init_tree() loop: w[k]=%f, k=%d, t=%d, n=%d, m=%d, q[t-1]=%f\n", w[k], k, t, n, m, q[t-1]);
        for (int j = 1; j <= t; j++) printf("q[%d]=%f ", j, q[j]); 
        printf("\n");
        #endif

        while (q[t-1]<=w[k]) combine(t, l, r, &t, w, v, q, &m);
        t++; 
        q[t] = w[k]; 
        v[t] = k;
    }

    while (t>1) combine(t, l, r, &t, w, v, q, &m);

    #ifdef DEBUG_PRINT
    for (int i = 0; i <= m; i++) {
        printf ("l[%d]=%d, r[%d]=%d\n", i, l[i], i, r[i]);
    }
     printf("v[1]=%d\n", v[1]);
    #endif

    mark(v[1], 0, d, l, r);

    t=0; m=2*n;
    build(1, d, l, r, &t, &m, nodes);

    assign_probs(nodes, n);
    #ifdef DEBUG_PRINT
    printf("INIT_TREE FINISHED\n");
    #endif

    #ifdef DEBUG_PRINT
    for (int j = 0; j <= n; j++) {
        printf("node %d, ", (*nodes)[j]->label);
    }
    printf("\n");
    for (int j = n+1; j<= 2*n; j++) {
        printf("node %d: G=%f, H=%f\n", (*nodes)[j]->label, (*nodes)[j]->G, (*nodes)[j]->H);
    }
    #endif

    return (*nodes)[2*n];
}


void print_tree(TreeNode* node, int depth) {
    if (node == NULL) return;
    
    // Print current node
    for (int i = 0; i < depth; i++) printf("-");
    printf("%d\n", node->label);

    // Print left subtree
    if (node->LLINK != NULL) {
        printf("L: ");
        print_tree(node->LLINK, depth + 1);
    }

    // Print right subtree
    if (node->RLINK != NULL) {
        printf("R: ");
        print_tree(node->RLINK, depth + 1);
    }
}

// Function to deallocate memory allocated for the tree nodes
void destroy_tree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    destroy_tree(root->LLINK);
    destroy_tree(root->RLINK);
    free(root);
}

