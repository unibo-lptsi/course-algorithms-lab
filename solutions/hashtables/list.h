/**
 * Module list: singly linked lists of TInfo (interface).
 *
 * A list is a pointer to its first node; the empty list is NULL.
 * The functions that may remove the first node return the new list, which the caller must store:
 *     L = list_delete_value(L, x);
 * The nodes are allocated (with malloc) by list_create, and freed by list_destroy (the whole list) or by
 * the deletions (the deleted node).
 */
#ifndef LIST_H
#define LIST_H

#include "tinfo.h"

typedef struct list {
    TInfo val;
    struct list *next;
} list;

/* These functions are already implemented (in list.c), so they can be used directly */

// Creates (with malloc) and returns a new node, holding val and followed by t (if malloc fails, it exits).
// The caller must free it when no longer used (e.g., with list_destroy).
list *list_create(TInfo val, list *t);
// Frees all the nodes of L
void list_destroy(list *L);
// Returns the number of nodes of L (0 if L is the empty list)
int list_length(list *L);
// Returns 1 iff L is the empty list, 0 otherwise
int list_is_empty(list *L);
// Returns 1 iff L1 and L2 hold the same values (as for tinfo_equal), in the same order
int list_equal(list *L1, list *L2);
// Returns a new list with n nodes, the i-th of which holds v[i] (node 0 is the first one)
list *list_from_array(TInfo v[], int n);
// Returns a copy of L (with new nodes)
list *list_clone(list *L);
// Returns a pointer to the value of the first node equal to value (as for tinfo_equal), or NULL if none
TInfo *list_search(list *L, TInfo value);
// Deletes (and frees) the first node equal to val (as for tinfo_equal), if any; returns the new list
list *list_delete_value(list *L, TInfo val);
// Print the values of L (L may be NULL) on stdout, as (v1 v2 ... vn) and as (v1,(v2,(...(vn,()))))
void list_print(list *L);
void list_printr(list *L);

#endif // LIST_H
