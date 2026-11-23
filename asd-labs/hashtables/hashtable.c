/**
 * Module hashtable: hash tables with chaining (implementation). The interface is in hashtable.h.
 *
 * Esercizio. Implementare le seguenti funzioni sull'implementazione di hasthable chained fornita:
 * 1) HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries) che crei e inizializzi una hashtable con le entry fornite
 * 2) HashTable *hashtable_merge(HashTable* h1, HashTable *h2) che restituisca una nuova hashtable data dall'unione delle due tabelle hash fornite in input.
 * Si scrivano i test per verificare le funzionalità implementate (test_init e test_merge in hashtable_test.c).
 */
#include "hashtable.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * REQUIRE(cond) checks a precondition of a public function, i.e. a condition that the caller must guarantee
 * (e.g., a positive number of buckets): if it is false, it reports where and which one, then stops the program
 * (abort). As in darray.c (lab dynamic-arrays), whose comment explains why not assert: compiling with -DNDEBUG
 * removes all the asserts, and an unchecked violation would go on silently (e.g., dividing by 0 when hashing).
 * NB: fflush(stdout) prints the pending output first: abort does not, so it could be lost
 * (e.g., when stdout is redirected to a file).
 */
#define REQUIRE(cond) do { \
        if(!(cond)) { \
            fflush(stdout); \
            fprintf(stderr, "%s:%d: %s: precondition violated: %s\n", __FILE__, __LINE__, __func__, #cond); \
            abort(); \
        } \
    } while(0)

// The hash function of the tables created from now on: static, i.e. private to this file (it can be changed with
// hashtable_set_hash_function); each table keeps the one in use when it is created (see hashtable.h)
static hash_function_type hash_function = &hash_int;

unsigned int hash_int(TKey key, int m) {
    int r = key % m; // NB: in C, % truncates toward zero, so r < 0 if key < 0 (e.g., -3 % 10 == -3)
    return (unsigned int)(r < 0 ? r + m : r); // mathematical "key mod m", in [0, m-1]
}

void hashtable_set_hash_function(hash_function_type f) {
    hash_function = f;
}

HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries) {
    return NULL; // TODO
}

HashTable *hashtable_merge(HashTable* h1, HashTable *h2) {
    return NULL; // TODO
}

HashTable *hashtable_create(int nbuckets) {
    REQUIRE(nbuckets > 0); // with 0 buckets, hashing would divide by 0 (key % 0)
    HashTable *h = (HashTable*) malloc(sizeof(HashTable));
    if(h == NULL) return NULL;
    h->bucket = (list**) malloc(sizeof(list*) * (size_t)nbuckets);
    if(h->bucket == NULL) { free(h); return NULL; }
    h->nbuckets = nbuckets;
    h->size = 0;
    h->hash = hash_function; // NB: the function in use now: changing it later does not change this table
    // NB: not calloc, which sets all the bits to 0: ISO C does not guarantee that this is a NULL pointer
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
    free(h);
}

list* hashtable_list(HashTable *h, TKey key) {
    return h->bucket[hashtable_hash(h, key)];
}

unsigned int hashtable_hash(HashTable *h, TKey key) {
    return h->hash(key, h->nbuckets);
}

// Returns the node of the chain l holding key, or NULL if there is none
static list* chain_search(list* l, TKey key) {
    for(; l != NULL; l = l->next) {
        if(l->val.key == key) {
            return l;
        }
    }
    return NULL;
}

void hashtable_insert(HashTable* h, TKey key, TValue val) {
    if(h == NULL) return;
    TInfo info = { .key = key, .value = val };
    unsigned int hash = hashtable_hash(h, key);
    list* list_item = chain_search(h->bucket[hash], key); // NB: not hashtable_search_list_item, which would hash again
    if(list_item == NULL) {
        h->bucket[hash] = list_create(info, h->bucket[hash]);
        h->size++;
    } else {
        list_item->val.value = val; // update value
    }
}

void hashtable_delete(HashTable* ht, TKey key) {
    if(ht == NULL) return;
    unsigned int h = hashtable_hash(ht, key);
    if(chain_search(ht->bucket[h], key) == NULL) return; // not present: nothing to delete (and size unchanged)
    TInfo ikey = { .key = key };
    ht->bucket[h] = list_delete_value(ht->bucket[h], ikey);
    ht->size--;
}

int hashtable_search_value(HashTable* h, TValue val) {
    if(h == NULL) return 0;
    for(int i = 0; i < h->nbuckets; i++) {
        list *l = h->bucket[i];
        for(; l != NULL; l = l->next) {
            if(l->val.value == val) return 1;
        }
    }
    return 0;
}

list *hashtable_search_list_item(HashTable* h, TKey key) {
    if(h == NULL) return NULL;
    return chain_search(hashtable_list(h, key), key);
}

TValue *hashtable_search(HashTable* h, TKey key) {
    list* l = hashtable_search_list_item(h, key); // NB: so, the keys are compared in one place only
    return l == NULL ? NULL : &l->val.value;
}

void hashtable_print(HashTable* h, int include_empty_buckets, const char* pre) {
    if(h == NULL) {
        printf("%s {}\n", pre);
        return;
    }
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
