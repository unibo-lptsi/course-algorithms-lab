/**
 * Module list: singly linked lists of TInfo (implementation). The interface is in list.h.
 */
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

/* Clona una lista (NB: ricorsiva, usa O(n) spazio sullo stack) */
list *list_clone(list *L) {
    if (L == NULL) return NULL;
    return list_create(L->val, list_clone(L->next));
}

/* Crea (mediante malloc() ) e restituisce un puntatore ad un nuovo
   nodo di una lista; il nodo contiene il valore v e punta a t come
   elemento successivo. Il chiamante e' responsabile per deallocare
   mediante free() o simili il blocco di memoria restituita da questa
   funzione, quando non piu' utilizzata */
list *list_create(TInfo val, list *t) {
    list *r = (list *)malloc(sizeof(list));
    if(r == NULL) {
        // as darray_create: returning NULL would make the callers lose (or truncate) their lists, silently
        fprintf(stderr, "ERROR: malloc failed; exiting.\n");
        exit(EXIT_FAILURE);
    }
    r->val = val;
    r->next = t;
    return r;
}

/* Restituisce la lunghezza (numero di nodi) della lista L; se L e' la
   lista vuota, restituisce 0 */
int list_length(list *L) {
    int n = 0;
    for (; L != NULL; L = L->next) { // iterative: the recursive version uses a stack frame per node
        n++;
    }
    return n;
}

/* Libera la memoria occupata da tutti i nodi della lista L */
void list_destroy(list *L) {
    while (L != NULL) { // iterative, as list_length
        list *next = L->next; // NB: read before free(L), after which L->next can no longer be read
        free(L);
        L = next;
    }
}


/* Restituisce 1 sse L1 e L2 contengono gli stessi valori (NB: ricorsiva, usa O(n) spazio sullo stack) */
int list_equal(list *L1, list *L2) {
    if (L1 == NULL || L2 == NULL) {
        return (L1 == NULL && L2 == NULL);
    } else {
        return (tinfo_equal(L1->val, L2->val) && list_equal(L1->next, L2->next));
    }
}

/* Stampa i valori contenuti nei nodi di L; si puo' avere L == NULL. */
void list_print(list *L) {
    printf("(");
    while (L != NULL) {
        tinfo_print(L->val);
        if (L->next != NULL) {
            printf(" ");
        }
        L = L->next;
    }
    printf(")");
}

/* Stampa i valori contenuti nei nodi di L; si puo' avere L == NULL. (NB: ricorsiva, usa O(n) spazio sullo stack) */
void list_printr(list *L) {
    if(L == NULL) {
        printf("()");
        return;
    }
    printf("(");
    tinfo_print(L->val);
    printf(",");
    list_printr(L->next);
    printf(")");
}

/* Restituisce 1 se e solo se L e' la lista vuota, 0 altrimenti */
int list_is_empty(list *L) { return (L == NULL); }

/* Restituisce una lista con n nodi, in cui il nodo i-esimo contiene
   il valore v[i] (il nodo 0 è il primo nodo della lista). (NB: ricorsiva, usa O(n) spazio sullo stack) */
list *list_from_array(TInfo v[], int n) {
    if (n > 0) {
        return list_create(v[0], list_from_array(v + 1, n - 1));
    } else {
        return NULL;
    }
}

TInfo *list_search(list *L, TInfo value) {
    for(; L != NULL; L = L->next) {
        if(tinfo_equal(value, L->val)) {
            return &L->val;
        }
    }
    return NULL;
}

list* list_delete_value(list *L, TInfo val) {
    if(L == NULL) return NULL;
    if(tinfo_equal(L->val, val)) {
        list *result = L->next;
        free(L);
        return result;
    }
    list *prev = L;
    list *curr = L;
    for(curr = curr->next; curr != NULL; curr = curr->next) {
        if(tinfo_equal(curr->val, val)) {
            prev->next = curr->next;
            free(curr);
            return L;
        }
        prev = curr;
    }
    return L;
}
