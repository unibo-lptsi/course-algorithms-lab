/**
 * Module hashtable: hash tables with chaining (implementation). The interface is in hashtable.h.
 *
 * Esercizio 1. Implementare le seguenti funzioni sull'implementazione di hashtable chained fornita:
 * 1) TValue *hashtable_search(HashTable* h, TKey key) che restituisca il puntatore al valore associato a key (NULL se assente)
 * 2) void hashtable_insert(HashTable* h, TKey key, TValue val) che inserisca l'entry key->val (o aggiorni il valore di key)
 * 3) void hashtable_add_many(HashTable* h, const TInfo* entries, int nentries) che inserisca in h le entry fornite, e
 *    HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries) che crei e inizializzi una hashtable con le entry fornite
 * 4) HashTable *hashtable_merge(HashTable* h1, HashTable *h2) che restituisca una nuova hashtable data dall'unione delle due tabelle hash fornite in input.
 * Si scrivano i test per verificare add_many, init e merge (test_init e test_merge in hashtable_test.c); quelli di
 * search e insert sono già scritti (test_search, test_insert, test_insert_existing_key).
 *
 * Esercizio 2 (extra, si veda README.md): implementare hashtable_rehash e l'espansione delle tabelle dinamiche in
 * hashtable_insert (si veda il TODO), verificandoli con test_rehash e test_dynamic_table.
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

void hashtable_add_many(HashTable* h, const TInfo* entries, int nentries) {
    REQUIRE(nentries >= 0);
    REQUIRE(entries != NULL || nentries == 0);
    // TODO (exercise 1): insert entries[0..nentries-1], in this order (see hashtable.h)
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
    h->max_load_factor = 0; // a fixed table: it never expands
    // NB: not calloc, which sets all the bits to 0: ISO C does not guarantee that this is a NULL pointer
    for(int i = 0; i < nbuckets; i++) {
        h->bucket[i] = NULL;
    }
    return h;
}

HashTable *hashtable_create_dynamic(int nbuckets, double max_load_factor) {
    REQUIRE(max_load_factor > 0); // 0 means a fixed table (and, if < 0, size > max_load_factor * nbuckets always)
    HashTable *h = hashtable_create(nbuckets);
    if(h != NULL) {
        h->max_load_factor = max_load_factor;
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


void hashtable_insert(HashTable* h, TKey key, TValue val) {
    // TODO (exercise 1): if key is already present, update its value; otherwise, insert the entry key->val at the
    // head of the chain of its bucket, and update size. If h is NULL, do nothing (see hashtable.h)
    // TODO (exercise 2): after inserting a new entry, if h is dynamic (max_load_factor > 0) and now
    // size > max_load_factor * nbuckets, expand it: about twice the buckets, i.e. the smallest prime >= 2 * nbuckets
    // (see README.md)
}

void hashtable_rehash(HashTable* h, int new_nbuckets) {
    REQUIRE(new_nbuckets > 0);
    // TODO (exercise 2)
    // 1. allocate the new bucket array
    // 2. rehash the entries of the old buckets into the new ones (move list nodes, do not copy them)
    // 3. free the old bucket array and update the fields of h
}

void hashtable_delete(HashTable* ht, TKey key) {
    if(ht == NULL) return;
    unsigned int h = hashtable_hash(ht, key);
    // NB: the list functions compare the entries with tinfo_equal, which compares their keys only
    TInfo ikey = { .key = key };
    if(list_search(ht->bucket[h], ikey) == NULL) return; // not present: nothing to delete (and size unchanged)
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

TValue *hashtable_search(HashTable* h, TKey key) {
    return NULL; // TODO (exercise 1)
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
