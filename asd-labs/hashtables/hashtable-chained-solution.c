#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Information pieces are key-value pairs */

typedef int TKey;
typedef int TValue;

typedef struct TInfo {
    TKey key;
    TValue value;
} TInfo;

int equal(TInfo a, TInfo b) {
    return a.key == b.key /* && a.value == b.value */; // NB: equality only based on keys
}

int info_repr(char *s, TInfo info) {
    return sprintf(s, "%d->%d", info.key, info.value);
}

/* list implementation */

typedef struct list {
    TInfo val;
    struct list *next;
} list;

/* Questi funzioni sono gia' definite, quindi e' possibile usarle direttamente */
list *list_create(TInfo val, list *t);
int list_length(list *L);
void list_destroy(list *L);
void list_print(list *L);
list *list_delete_value(list *L, TInfo val);
list *list_delete(list *L, int (*pred)(TInfo));
int is_empty(list *L);
list *list_from_array(TInfo v[], int n);
list *list_clone(list *list);
TInfo *list_search(list *list, TInfo value);

/* Clona una lista */
list *list_clone(list *list) {
    if (list == NULL) return NULL;
    return list_create(list->val, list_clone(list->next));
}

/* Crea (mediante malloc() ) e restituisce un puntatore ad un nuovo
   nodo di una lista; il nodo contiene il valore v e punta a t come
   elemento successivo. Il chiamante e' responsabile per deallocare
   mediante free() o simili il blocco di memoria restituita da questa
   funzione, quando non piu' utilizzata */
list *list_create(TInfo val, list *t) {
    list *r = (list *)malloc(sizeof(list));
    if(r == NULL) return NULL;
    r->val = val;
    r->next = t;
    return r;
}

/* Restituisce la lunghezza (numero di nodi) della lista L; se L e' la
   lista vuota, restituisce 0 */
int list_length(list *L) {
    if (NULL == L) {
        return 0;
    } else {
        return (1 + list_length(L->next));
    }
}

/* Libera la memoria occupata da tutti i nodi della lista L */
void list_destroy(list *L) {
    if (L != NULL) {
        list_destroy(L->next);
        L->next = NULL; /* non necessario... */
        free(L);
    }
}


/* Restituisce 1 sse L1 e L2 contengono gli stessi valori */
int list_equal(list *L1, list *L2) {
    if (L1 == NULL || L2 == NULL) {
        return (L1 == NULL && L2 == NULL);
    } else {
        return (equal(L1->val, L2->val) && list_equal(L1->next, L2->next));
    }
}

/* Stampa i valori contenuti nei nodi di L; si puo' avere L == NULL. */
void list_print(list *L) {
    printf("(");
    while (L != NULL) {
        char s[20];
        info_repr(s, L->val);
        printf("%s", s);
        if (L->next != NULL) {
            printf(" ");
        }
        L = L->next;
    }
    printf(")");
}

/* Stampa i valori contenuti nei nodi di L; si puo' avere L == NULL. */
void list_printr(list *L) {
    if(L == NULL) { 
        printf("()");
        return;
    }
    printf("(");
    char s[20];
    info_repr(s, L->val);
    printf("%s,", s);
    list_printr(L->next);
    printf(")");
}

/* Restituisce 1 se e solo se L e' la lista vuota, 0 altrimenti */
int is_empty(list *L) { return (L == NULL); }

/* Restituisce una lista con n nodi, in cui il nodo i-esimo contiene
   il valore v[i] (il nodo 0 è il primo nodo della lista). */
list *list_from_array(TInfo v[], int n) {
    if (n > 0) {
        return list_create(v[0], list_from_array(v + 1, n - 1));
    } else {
        return NULL;
    }
}

TInfo *list_search(list *list, TInfo value) {
    for(; list != NULL; list = list->next) {
        if(equal(value, list->val)) {
            return &list->val;
        }
    }
    return NULL;
}

list* list_delete(list *L, int (*pred)(TInfo)) {
    if(L == NULL) return NULL;
    if(pred(L->val)) { 
        list *result = L->next;
        free(L);
        return result;
    }
    list *prev = L;
    list *curr = L;
    for(curr = curr->next; curr != NULL; curr = curr->next) {
        if(pred(curr->val)) {
            prev->next = curr->next;
            free(curr);
            return L;
        }
        prev = L;
    }
    return L;
}

list* list_delete_value(list *L, TInfo val) {
    if(L == NULL) return NULL;
    if(equal(L->val, val)) { 
        list *result = L->next;
        free(L);
        return result;
    }
    list *prev = L;
    list *curr = L;
    for(curr = curr->next; curr != NULL; curr = curr->next) {
        if(equal(curr->val, val)) {
            prev->next = curr->next;
            free(curr);
            return L;
        }
        prev = L;
    }
    return L;
}

/* Chained Hashtable implementation (closed addressing) */

typedef unsigned int(*hash_function_type)(TKey);
unsigned int hash_int(TKey key);

typedef struct HashTable {
    list** bucket;
    int nbuckets;
} HashTable;

static hash_function_type hash_function = &hash_int;

unsigned int hash_int(TKey key) {
    return key;
}

HashTable *hashtable_create(int nbuckets);
unsigned int hashtable_hash(HashTable *h, TKey key);
list* hashtable_list(HashTable *h, TKey key);
void hashtable_destroy(HashTable* h);
void hashtable_insert(HashTable* h, TKey key, TValue val);
void hashtable_delete(HashTable* h, TKey key);
void hashtable_delete_value(HashTable* h, TKey key, TValue val);
TValue *hashtable_search(HashTable* h, TKey key);
int hashtable_search_value(HashTable* h, TValue val);
int hashtable_search_keyvalue(HashTable* h, TKey key, TValue val);
void hashtable_print(HashTable* h, int include_empty_buckets, char *pre);
HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries); // to be implemented
HashTable *hashtable_merge(HashTable* h1, HashTable *h2); // to be implemented

HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries) {
    HashTable* ht = hashtable_create(nbuckets);
    for(int i = 0; i < nentries; i++) {
        hashtable_insert(ht, entries[i].key, entries[i].value);
    }
    return ht;
}

HashTable *hashtable_merge(HashTable* h1, HashTable *h2) {
    HashTable *res = hashtable_create(h1->nbuckets + h2->nbuckets);
    for(int i = 0; i < h1->nbuckets; i++) {
        for(list *l = h1->bucket[i]; l != NULL; l = l->next) {
            TInfo entry = l->val;
            hashtable_insert(res, entry.key, entry.value);            
        }
    }
    for(int i = 0; i < h2->nbuckets; i++) {
        for(list *l = h2->bucket[i]; l != NULL; l = l->next) {
            TInfo entry = l->val;
            hashtable_insert(res, entry.key, entry.value);            
        }
    }
    return res;
}

HashTable *hashtable_create(int nbuckets) {
    HashTable *h = (HashTable*) malloc(sizeof(HashTable));
    if(h == NULL) return NULL;
    h->bucket = (list**) malloc(sizeof(list*) * nbuckets);
    if(h->bucket == NULL) { free(h); return NULL; }
    h->nbuckets = nbuckets;
    for(int i = 0; i < nbuckets; i++) {
        h->bucket[i] = NULL;
    }
    return h;
}

void hashtable_destroy(HashTable* h) {
    if(h == NULL) return;
    for(int i = 0; i < h->nbuckets; i++) {
        list_destroy(h->bucket[i]);
    }
    free(h->bucket);
    h->nbuckets = 0;
    free(h);
}

list* hashtable_list(HashTable *h, TKey key) {
    return h->bucket[hashtable_hash(h, key)];
}

unsigned int hashtable_hash(HashTable *h, TKey key) {
    return hash_function(key) % h->nbuckets;
}

void hashtable_insert(HashTable* h, TKey key, TValue val) {
    TInfo info = { key = key, val = val };
    unsigned int hash = hashtable_hash(h, key);
    if(!hashtable_search_keyvalue(h, key, val)) {
        h->bucket[hash] = list_create(info, h->bucket[hash]);
    }
}

void hashtable_delete(HashTable* ht, TKey key) {
    unsigned int h = hashtable_hash(ht, key);
    TInfo ikey = { key = key };
    ht->bucket[h] = list_delete_value(ht->bucket[h], ikey);
}

void hashtable_delete_value(HashTable* h, TKey key, TValue val) {
    unsigned int hash = hashtable_hash(h, key);
    list* l = h->bucket[hash];
    if(l != NULL) {
        TInfo info = { key = key, val = val };
        h->bucket[hash] = list_delete_value(l, info); // must be assigned since first element might need to be removed
    }
}

int hashtable_search_value(HashTable* h, TValue val) {
    if(h == NULL) return 0;
    for(int i = 0; i < h->nbuckets; i++) {
        list *list = h->bucket[i];
        for(; list != NULL; list = list->next) {
            if(list->val.value == val) return 1;
        }
    }
    return 0;
}

int hashtable_search_keyvalue(HashTable* h, TKey key, TValue val) {
    TInfo info = { key = key, val = val };
    TInfo *value_in_list = list_search(hashtable_list(h, key), info);
    return value_in_list != NULL && equal(info, *value_in_list);
}

TValue *hashtable_search(HashTable* h, TKey key) {
    list* l = hashtable_list(h, key);
    for(; l != NULL; l = l->next) {
        if(l->val.key == key) {
            return &l->val.value;
        }
    }
    return NULL;
}

void hashtable_print(HashTable* h, int include_empty_buckets, char* pre) {
    printf("%s {\n", pre);
    for(int i = 0; i < h->nbuckets; i++) {
        if(include_empty_buckets || h->bucket[i] != NULL) {
            printf("\t[%d] ", i);
            list_print(h->bucket[i]);
            printf("\n");
        }
    }
    printf("}\n");
}

int main(void) {
    HashTable *h = hashtable_create(10);
    hashtable_insert(h, 4, 77);
    hashtable_insert(h, 44, 66);
    hashtable_insert(h, 85, 99);
    hashtable_insert(h, 175, 55);
    hashtable_print(h, 1, "hashtable (showing all buckets) =");
    hashtable_print(h, 0, "hashtable =");
    printf("Is value 55 present? %s.\n", hashtable_search_value(h, 55) ? "yes" : "no");
    printf("Is value 371 present? %s.\n", hashtable_search_value(h, 371) ? "yes" : "no");
    hashtable_delete(h, 4);
    hashtable_print(h, 0, "after removal of key 4 =");
    hashtable_delete_value(h, 175, 55);
    hashtable_print(h, 0, "after removal of element 55 =");

    // Testing new functionality
    HashTable *h2 = hashtable_init(10, (TInfo[]) { { 3, 7}, { 4, 8}, {5, 9}, { 6, 10 } }, 4);
    hashtable_print(h2, 0, "h2");

    HashTable *h3 = hashtable_merge(h, h2);
    hashtable_print(h3, 0, "h3");

    hashtable_destroy(h);
    hashtable_destroy(h2);
    hashtable_destroy(h3);
    return 0;
}
