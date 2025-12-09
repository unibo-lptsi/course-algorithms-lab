#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * Binary Search Trees (BST) esercizio. Implementare le funzioni
 * - bst_init
 * - bst_height
 * - bst_count_leaves
 * - bst_visit_postorder
 */

// NB: compile with gcc "-DDEBUG"
#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stdout, "[LOG] " fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

typedef int TInfo;
typedef struct SBSTNode {
    TInfo info;
    struct SBSTNode *left;
    struct SBSTNode *right;
} BSTNode;
typedef BSTNode* BSTree;

bool equal(TInfo a, TInfo b) {
    return a == b;
}

bool less(TInfo a, TInfo b) {
    return a < b;
}

// Operations on nodes
BSTNode *bst_node_create(TInfo value);
void bst_node_destroy(BSTNode* node);

// Creation and destruction of btrees
BSTree bst_create();
void bst_destroy(BSTree tree);

// Operations on btrees
void bst_visit(BSTree tree, void (*f)(TInfo));
void bst_visit_preorder(BSTree tree, void (*f)(TInfo));
void bst_visit_postorder(BSTree tree, void (*f)(TInfo));
void bst_print_preorder(BSTree tree, int level);
BSTNode *bst_search(BSTree tree, TInfo value);
BSTree bst_insert(BSTree tree, TInfo info);
BSTree bst_delete(BSTree tree, TInfo info);
BSTNode *bst_search_min(BSTree tree);
bool bst_is_empty(BSTree tree);
int bst_height(BSTree tree);
int bst_sum(BSTree tree);
int bst_count_nodes(BSTree tree);
int bst_count_leaves(BSTree tree);

BSTree bst_init(TInfo* entries, int nentries) {
    return NULL; // TODO: implement this
}

int bst_height(BSTree tree) {
    return -1; // TODO: implement this
}

int bst_count_leaves(BSTree tree) {
    return -1; // TODO: implement this
}

void bst_visit_postorder(BSTree tree, void (*f)(TInfo)) {
    // TODO: implement this
}


BSTNode *bst_node_create(TInfo value) {
    BSTNode *node = (BSTNode*) malloc(sizeof(BSTNode));
    node->info = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void bst_node_destroy(BSTNode* node) {
    free(node);
}

/* Returns an empty binary tree */
BSTree bst_create() {
    return NULL;
}

void bst_destroy(BSTree tree) {
    if(tree != NULL) {
        bst_destroy(tree->left);
        tree->left = NULL;
        bst_destroy(tree->right);
        tree->right = NULL;
        bst_node_destroy((BSTNode*) tree);
    }
}

// Operations on btrees
void bst_visit(BSTree tree, void (*f)(TInfo)) {
    if(tree != NULL) {
        bst_visit(tree->left, f);
        f(tree->info);
        bst_visit(tree->right, f);
    }
}

void bst_visit_preorder(BSTree tree, void (*f)(TInfo)) {
    if(tree != NULL) {
        f(tree->info);
        bst_visit_preorder(tree->left, f);
        bst_visit_preorder(tree->right, f);
    }
}

char *spacing(int level, char* s) {
    int i=0;
    for(; i<(level * 2); i++) {
        s[i] = ' ';
    }
    s[i] = '\0';
    return s;
}

void bst_print_preorder(BSTree tree, int level) {
    if(tree != NULL) {
        char c[200];
        printf("[%d]", tree->info);
        spacing(level+1, c);
        if(tree->left != NULL) printf("\n%s L: ", c);
        bst_print_preorder(tree->left, level+1);
        if(tree->right != NULL) printf("\n%s R: ", c);
        bst_print_preorder(tree->right, level+1);
    }
}

BSTree bst_insert(BSTree tree, TInfo info) {
    if(tree == NULL) {
        return bst_node_create(info);
    } else {
        if(less(info, tree->info) || equal(info, tree->info) ) {
            tree->left = bst_insert(tree->left, info);
        } else {
            tree->right = bst_insert(tree->right, info);
        }
        return tree;
    }
}

BSTNode *bst_search(BSTree tree, TInfo value) {
    if(tree == NULL || equal(value, tree->info)) return tree;
    return bst_search(less(value, tree->info) ? tree->left : tree->right, value);
}

BSTNode *bst_search_min(BSTree tree) {
    if(tree == NULL || tree->left == NULL) return tree;
    return bst_search_min(tree->left);
}

BSTree bst_delete(BSTree tree, TInfo info) {
    if(tree == NULL) return tree;
    if(equal(tree->info, info)) {
        if (tree->left != NULL && tree->right != NULL) {
            BSTree min = bst_search_min(tree->right);
            tree->info = min->info;
            tree->right = bst_delete(min->right, min->info);
            return tree;
        } else if(tree->left == NULL && tree->right == NULL) {
            bst_node_destroy(tree);
            return NULL;
        } else {
            BSTree result = tree->left ? tree->left : tree->right;
            bst_node_destroy(tree);
            return result;
        }
    } else {
        if(less(tree->info, info)) {
            tree->right = bst_delete(tree->right, info);
        } else { 
            tree->left = bst_delete(tree->left, info);
        }
        return tree;
    }
}

int bst_sum(BSTree tree) {
    if(!tree) return 0;
    int sum_left = bst_sum(tree->left);
    int sum_right = bst_sum(tree->right);
    return tree->info + sum_left + sum_right;
}

bool bst_is_empty(BSTree tree) {
    return tree == NULL;
}

void print_node(TInfo i) {
    printf("(%d)", i);
}

int bst_count_nodes(BSTree tree) {
    if(!tree) return 0;
    int nnodes_left = bst_count_nodes(tree->left);
    int nnodes_right = bst_count_nodes(tree->right);
    return 1 + nnodes_left + nnodes_right;
}

void test() {
    puts("### Part 1\n\nInserting 10, 7, 15, 3 in order on empty tree. Printing tree:");
    BSTree tree = bst_insert(NULL, 10);
    tree = bst_insert(tree, 7);
    tree = bst_insert(tree, 15);
    tree = bst_insert(tree, 3);
    bst_print_preorder(tree, 0);
    printf("\nSymmetric (in-order) visit: ");
    bst_visit(tree, print_node);
    printf("\nPreorder visit: ");
    bst_visit_preorder(tree, print_node);
    printf("\nPostorder visit: ");
    bst_visit_postorder(tree, print_node);
    puts("");
    printf("Is 3 included in the tree? %s.\n", bst_search(tree, 3) != NULL ? "yes" : "no");
    printf("Is 88 included? %s.\n", bst_search(tree, 88) != NULL ? "yes" : "no");
    BSTNode* min = bst_search_min(tree);
    printf("Is there some minimum value? %s. ", min != NULL ? "yes" : "no");
    if(min != NULL) printf("It is %d.", min->info);
    puts("\nInserting 1:");
    tree = bst_insert(tree, 1);
    bst_visit(tree, print_node);
    min = bst_search_min(tree);
    puts("");
    printf("Is there some minimum value? %s. ", min != NULL ? "yes" : "no");
    if(min != NULL) printf("It is %d.", min->info);
    puts("");
    bst_print_preorder(tree, 0);
    puts("\nAfter removal of 7:");
    tree = bst_delete(tree, 7);
    bst_print_preorder(tree, 0);
    puts("\nAfter removal of 3:");
    tree = bst_delete(tree, 3);
    bst_print_preorder(tree, 0);
    puts("");
    puts("\nAfter removal of 10:");
    tree = bst_delete(tree, 10);
    bst_print_preorder(tree, 0);
    puts("");
    bst_destroy(tree);

    puts("\n### Part 2\n\nA new BST (simplified creation via bst_init):");
    BSTree t2 = bst_init((TInfo[]) { 5, 2, 8, 0, 6, 4, 10 }, 7); 
    bst_print_preorder(t2, 0);
    puts("");
    printf("The tree has height %d.\n", bst_height(t2));
    printf("The tree has %d nodes.\n", bst_count_nodes(t2));
    printf("The tree has %d leaves.\n", bst_count_leaves(t2));
    printf("The tree has sum %d.\n", bst_sum(t2));
    puts("");
}

int main() {
    test();
    return 0;
}