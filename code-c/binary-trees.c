#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int TInfo;
typedef struct SNode {
    TInfo info;
    struct SNode *left;
    struct SNode *right;
} TNode;
typedef TNode* TBinaryTree;

bool equal(TInfo a, TInfo b) {
    return a == b;
}

bool less(TInfo a, TInfo b) {
    return a < b;
}

// Operations on nodes
TNode *node_create(TInfo value);
void node_destroy(TNode* node);

// Creation and destruction of btrees
TBinaryTree binarytree_create();
TBinaryTree binarytree_destroy();

// Operations on btrees
void binarytree_visit(TBinaryTree tree, void (*f)(TInfo));
void binarytree_visit_preorder(TBinaryTree tree, void (*f)(TInfo));
void binarytree_visit_postorder(TBinaryTree tree, void (*f)(TInfo));
TNode *binarytree_search(TBinaryTree tree, TInfo value);
TNode *binarytree_search(TBinaryTree tree, TInfo value);
TBinaryTree binarytree_insert(TBinaryTree tree, TInfo info);
TBinaryTree binarytree_delete(TBinaryTree tree, TInfo info);
bool binarytree_is_empty(TBinaryTree tree);
int binarytree_height(TBinaryTree tree);
int binarytree_sum(TBinaryTree tree);
int binarytree_count_nodes(TBinaryTree tree);
int binarytree_count_leaves(TBinaryTree tree);

TNode *node_create(TInfo value) {
    TNode *node = (TNode*) malloc(sizeof(TNode));
    node->info = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void node_destroy(TNode* node) {
    free(node);
}

/* Returns an empty binary tree */
TBinaryTree binarytree_create() {
    return NULL;
}

TBinaryTree binarytree_destroy(TBinaryTree tree) {
    if(tree != NULL) {
        tree->left = binarytree_destroy(tree->left);
        tree->right = binarytree_destroy(tree->right);
        node_destroy((TNode*) tree);
    }
    return NULL;
}

// Operations on btrees
void binarytree_visit(TBinaryTree tree, void (*f)(TInfo)) {
    if(tree != NULL) {
        binarytree_visit(tree->left, f);
        f(tree->info);
        binarytree_visit(tree->right, f);
    }
}

void binarytree_visit_preorder(TBinaryTree tree, void (*f)(TInfo)) {
    if(tree != NULL) {
        f(tree->info);
        binarytree_visit_preorder(tree->left, f);
        binarytree_visit_preorder(tree->right, f);
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

void binarytree_print_preorder(TBinaryTree tree, int level) {
    if(tree != NULL) {
        char c[200];
        printf("[%d]", tree->info);
        spacing(level+1, c);
        if(tree->left != NULL) printf("\n%s L: ", c);
        binarytree_print_preorder(tree->left, level+1);
        if(tree->right != NULL) printf("\n%s R: ", c);
        binarytree_print_preorder(tree->right, level+1);
    }
}

void binarytree_visit_postorder(TBinaryTree tree, void (*f)(TInfo)) {
    if(tree != NULL) {
        binarytree_visit_postorder(tree->left, f);
        binarytree_visit_postorder(tree->right, f);
        f(tree->info);
    }
}

TBinaryTree binarytree_insert(TBinaryTree tree, TInfo info) {
    if(tree == NULL) {
        return node_create(info);
    } else {
        if(less(info, tree->info) || equal(info, tree->info) ) {
            tree->left = binarytree_insert(tree->left, info);
        } else {
            tree->right = binarytree_insert(tree->right, info);
        }
        return tree;
    }
}

TNode *binarytree_search(TBinaryTree tree, TInfo value) {
    if(tree == NULL || equal(value, tree->info)) return tree;
    return binarytree_search(less(value, tree->info) ? tree->left : tree->right, value);
}

TNode *binarytree_search_min(TBinaryTree tree) {
    if(tree == NULL || tree->left == NULL) return tree;
    return binarytree_search_min(tree->left);
}

TBinaryTree binarytree_delete(TBinaryTree tree, TInfo info) {
    if(tree == NULL) return tree;
    if(equal(tree->info, info)) {
        if (tree->left != NULL && tree->right != NULL) {
            TBinaryTree min = binarytree_search_min(tree->right);
            tree->info = min->info;
            tree->right = binarytree_delete(min->right, min->info);
            return tree;
        } else if(tree->left == NULL && tree->right == NULL) {
            node_destroy(tree);
            return NULL;
        } else {
            TBinaryTree result = tree->left ? tree->left : tree->right;
            node_destroy(tree);
            return result;
        }
    } else {
        if(less(tree->info, info)) {
            tree->right = binarytree_delete(tree->right, info);
        } else { 
            tree->left = binarytree_delete(tree->left, info);
        }
        return tree;
    }
}

TBinaryTree binarytree_init(TInfo* entries, int nentries) {
    TBinaryTree t = NULL;
    for(int i = 0; i < nentries; i++) {
        t = binarytree_insert(t, entries[i]);
    }
    return t;
}

int binarytree_sum(TBinaryTree tree) {
    if(!tree) return 0;
    int sum_left = binarytree_sum(tree->left);
    int sum_right = binarytree_sum(tree->right);
    return tree->info + sum_left + sum_right;
}

bool binarytree_is_empty(TBinaryTree tree) {
    return tree == NULL;
}

void print_node(TInfo i) {
    printf("(%d)", i);
}

int binarytree_height(TBinaryTree tree) {
    if(binarytree_is_empty(tree)) return -1;
    int hleft = binarytree_height(tree->left);
    int hright = binarytree_height(tree->right);
    return hleft >= hright ? hleft + 1 : hright + 1;
}

int binarytree_count_nodes(TBinaryTree tree) {
    if(!tree) return 0;
    int nnodes_left = binarytree_count_nodes(tree->left);
    int nnodes_right = binarytree_count_nodes(tree->right);
    return 1 + nnodes_left + nnodes_right;
}

int binarytree_count_leaves(TBinaryTree tree) {
    if(!tree) return 0;
    if(!tree->left && !tree->right) return 1;
    return binarytree_count_leaves(tree->left) + binarytree_count_leaves(tree->right);
}

int main() {
    TBinaryTree tree = binarytree_insert(NULL, 10);
    tree = binarytree_insert(tree, 7);
    tree = binarytree_insert(tree, 15);
    tree = binarytree_insert(tree, 3);
    binarytree_print_preorder(tree, 0);
    puts("");
    puts("Symmetric visit: ");
    binarytree_visit(tree, print_node);
    puts("\nPreorder visit: ");
    binarytree_visit_preorder(tree, print_node);
    puts("\nPostorder visit: ");
    binarytree_visit_postorder(tree, print_node);
    puts("");
    printf("E' presente il valore 3? %s.\n", binarytree_search(tree, 3) != NULL ? "yes" : "no");
    printf("E' presente il valore 88? %s.\n", binarytree_search(tree, 88) != NULL ? "yes" : "no");
    TNode* min = binarytree_search_min(tree);
    printf("Il valore minimo è presente? %s. ", min != NULL ? "yes" : "no");
    if(min != NULL) printf("E vale %d.", min->info);
    puts("\nInserimento valore 1:");
    tree = binarytree_insert(tree, 1);
    binarytree_visit(tree, print_node);
    min = binarytree_search_min(tree);
    puts("");
    printf("Il valore minimo è presente? %s. ", min != NULL ? "yes" : "no");
    if(min != NULL) printf("E vale %d.", min->info);
    puts("");
    binarytree_print_preorder(tree, 0);
    puts("\nDopo cancellazione nodo 7:");
    tree = binarytree_delete(tree, 7);
    binarytree_print_preorder(tree, 0);
    puts("\nDopo cancellazione nodo 3:");
    tree = binarytree_delete(tree, 3);
    binarytree_print_preorder(tree, 0);
    puts("");
    puts("\nDopo cancellazione nodo 15:");
    tree = binarytree_delete(tree, 15);
    binarytree_print_preorder(tree, 0);
    puts("");
    binarytree_destroy(tree);

    puts("\nNuovo BST (creazione semplificata):");
    TBinaryTree t2 = binarytree_init((TInfo[]) { 5, 2, 8, 0, 6, 4, 10 }, 7); 
    binarytree_print_preorder(t2, 0);
    puts("");
    printf("L'albero ha altezza %d.\n", binarytree_height(t2));
    printf("L'albero ha %d nodi.\n", binarytree_count_nodes(t2));
    printf("L'albero ha %d foglie.\n", binarytree_count_leaves(t2));
    printf("L'albero ha come somma dei valori dei nodi %d.\n", binarytree_sum(t2));
    puts("");
    return 0;
}