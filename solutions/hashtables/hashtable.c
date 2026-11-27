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
    for(int i = 0; i < nentries; i++) {
        hashtable_insert(h, entries[i].key, entries[i].value); // NB: if h is NULL, it does nothing
    }
}

HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries) {
    HashTable *h = hashtable_create(nbuckets);
    hashtable_add_many(h, entries, nentries); // NB: if h is NULL (no memory), it does nothing
    return h;
}

// Inserts the entries of h into target (for the keys already in target, the value in h wins); h is left unchanged,
// and target gets nodes of its own. A NULL h is an empty table, as in the other operations.
// NB: each entry is re-inserted (rehashing), not copied bucket by bucket: if target has another number of buckets,
// a key may go to another bucket (k mod m depends on m)
static void hashtable_copy_to(HashTable* h, HashTable* target) {
    if(h == NULL) return;
    for(int i = 0; i < h->nbuckets; i++) {
        for(list *l = h->bucket[i]; l != NULL; l = l->next) {
            hashtable_insert(target, l->val.key, l->val.value);
        }
    }
}

HashTable *hashtable_merge(HashTable* h1, HashTable *h2) {
    HashTable *res = hashtable_create((h1 ? h1->nbuckets : 1) + (h2 ? h2->nbuckets : 1));
    if(res == NULL) return NULL;
    hashtable_copy_to(h1, res);
    hashtable_copy_to(h2, res); // after h1: for the keys in both, the value in h2 wins
    return res;
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

// Returns the node of the chain l holding key, or NULL if there is none (used by hashtable_search and
// hashtable_insert: so, in this file, the keys are compared in one place only)
static list* chain_search(list* l, TKey key) {
    for(; l != NULL; l = l->next) {
        if(l->val.key == key) {
            return l;
        }
    }
    return NULL;
}

// Returns 1 iff n is prime (trial division by 2, 3, ..., up to the square root of n), 0 otherwise
static int is_prime(int n) {
    if(n < 2) return 0;
    for(int d = 2; d <= n / d; d++) { // NB: d <= n / d, rather than d * d <= n, which could overflow
        if(n % d == 0) return 0;
    }
    return 1;
}

// Returns the smallest prime >= n
static int next_prime(int n) {
    while(!is_prime(n)) {
        n++;
    }
    return n;
}

// Expands the dynamic table h, whose load factor exceeds max_load_factor: about twice the buckets, i.e. the smallest
// prime >= 2 * nbuckets (with the division method, CLRS suggests a prime m, not close to a power of 2).
// NB: a geometric growth, as in darray: inserting n entries costs O(n) time for all the expansions, i.e. O(1)
// amortized per insertion; a linear one (e.g., m + 10) would cost Theta(n^2) time overall
static void expand(HashTable* h) {
    int m = h->nbuckets;
    do {
        m = next_prime(2 * m);
    } while(h->size > h->max_load_factor * m); // NB: once is enough, unless max_load_factor * nbuckets < 1
    hashtable_rehash(h, m);
}

void hashtable_insert(HashTable* h, TKey key, TValue val) {
    if(h == NULL) return;
    TInfo info = { .key = key, .value = val };
    unsigned int hash = hashtable_hash(h, key);
    list* list_item = chain_search(h->bucket[hash], key); // NB: not hashtable_search, which would hash again
    if(list_item == NULL) {
        h->bucket[hash] = list_create(info, h->bucket[hash]); // at the head of the chain: O(1)
        h->size++;
        // NB: 0 in a fixed table, which never expands (and size > 0 * nbuckets would always hold)
        if(h->max_load_factor > 0 && h->size > h->max_load_factor * h->nbuckets) {
            expand(h); // NB: after inserting the new entry, which is moved too
        }
    } else {
        list_item->val.value = val; // update value
    }
}

void hashtable_rehash(HashTable* h, int new_nbuckets) {
    REQUIRE(new_nbuckets > 0);
    if(h == NULL) return;
    // 1. allocate the new bucket array
    list** new_bucket = (list**) malloc(sizeof(list*) * (size_t)new_nbuckets);
    if(new_bucket == NULL) return; // h is left unchanged: still correct, only with longer chains
    for(int i = 0; i < new_nbuckets; i++) {
        new_bucket[i] = NULL;
    }
    // 2. rehash the entries of the old buckets into the new ones (move list nodes, do not copy them)
    // NB: the chains cannot be copied bucket by bucket (nor the bucket array resized with realloc, as in darray):
    // with another number of buckets, a key may hash to another bucket (k mod m depends on m)
    for(int i = 0; i < h->nbuckets; i++) {
        list* l = h->bucket[i];
        while(l != NULL) {
            list* next = l->next; // NB: read before l->next is changed
            unsigned int j = h->hash(l->val.key, new_nbuckets); // NB: not hashtable_hash, which uses h->nbuckets
            l->next = new_bucket[j]; // l becomes the head of its new chain
            new_bucket[j] = l;
            l = next;
        }
    }
    // 3. free the old bucket array and update the fields of h
    free(h->bucket); // NB: only the old bucket array: its nodes are now in the new one
    h->bucket = new_bucket;
    h->nbuckets = new_nbuckets;
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
    if(h == NULL) return NULL;
    list* l = chain_search(hashtable_list(h, key), key);
    return l == NULL ? NULL : &l->val.value; // NB: the address of the value in the node, not of a copy
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
