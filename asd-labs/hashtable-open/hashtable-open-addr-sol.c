#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * Esercizio: completare l'implementazione di una hashtable con open addressing e linear probing
 * 1- completare l'implementazione della funzione hashtable_expand_and_rehash(HashTable* h) (vedi TODO al suo interno)
 * 2- implementare las funzione hashtable_delete(HashTable* ht, TKey key)
 */

/* Information pieces are key-value pairs */

typedef enum { EMPTY, OCCUPIED, DELETED } TSlot;
typedef int TKey;
typedef int TValue;

typedef struct TInfo {
    TSlot status;
    TKey key;
    TValue value;
} TInfo;

int equal(TInfo a, TInfo b) {
    return a.key == b.key /* && a.value == b.value */; // NB: equality only based on keys
}

int equal_key(TKey a, TKey b) {
    return a == b;
}

int equal_value(TValue a, TValue b) {
    return a == b;
}

int info_repr(char *s, TInfo info) {
    return sprintf(s, "%d->%d", info.key, info.value);
}

// NB: compile with gcc "-DDEBUG"
#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif


/* Open addressing Hashtable implementation with linear probing */

static const int HASHTABLE_GROWTH_FACTOR = 2;

typedef unsigned int(*hash_function_type)(TKey);
unsigned int hash_int(TKey key);

typedef struct HashTable {
    TInfo* buckets;
    int n_buckets;
    int size;
} HashTable;

static hash_function_type hash_function = &hash_int;

unsigned int hash_int(TKey key) {
    return key;
}

HashTable *hashtable_create(int nbuckets);
unsigned int hashtable_hash(HashTable *h, TKey key);
void hashtable_destroy(HashTable* h);
void hashtable_insert(HashTable* h, TKey key, TValue val);
void hashtable_delete(HashTable* h, TKey key);
TValue *hashtable_search(HashTable* h, TKey key);
int hashtable_exists_value(HashTable* h, TValue val);
int hashtable_search_keyvalue(HashTable* h, TKey key, TValue val);
void hashtable_print(HashTable* h, int include_empty_buckets, char *pre);
HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries);

HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries) {
    HashTable* ht = hashtable_create(nbuckets);
    for(int i = 0; i < nentries; i++) {
        hashtable_insert(ht, entries[i].key, entries[i].value);
    }
    return ht;
}

HashTable *hashtable_create(int nbuckets) {
    HashTable *h = (HashTable*) malloc(sizeof(HashTable));
    if(h == NULL) return NULL;
    h->buckets = (TInfo*) malloc(sizeof(TInfo) * nbuckets);
    if(h->buckets == NULL) { free(h); return NULL; }
    h->n_buckets = nbuckets;
    for(int i = 0; i < nbuckets; i++) {
        h->buckets[i] = (TInfo) { .status = EMPTY, .key = 0, .value = 0 };
    }
    return h;
}

void hashtable_destroy(HashTable* h) {
    if(h == NULL) return;
    free(h->buckets);
    free(h);
}

unsigned int hashtable_hash(HashTable *h, TKey key) {
    return hash_function(key) % h->n_buckets;
}

int hashtable_expand_and_rehash(HashTable* h) {
    int new_n_buckets = h->n_buckets * HASHTABLE_GROWTH_FACTOR;
    // N.B. could also use realloc() but that would require cloning the array, re-initialising to zero the original reallocated array, and rehashing
    TInfo* new_buckets = (TInfo*) malloc(sizeof(TInfo) * new_n_buckets);
    if(new_buckets == NULL) {
        LOG("  hashtable_expand_and_rehash: failed to allocate new buckets\n");
        return 0; // failed
    }
    for(int i = 0; i < new_n_buckets; i++) {
        new_buckets[i] = (TInfo) { .status = EMPTY, .key = 0, .value = 0 };
    }
    // rehash existing entries
    for(int i = 0; i < h->n_buckets; i++) {
        if(h->buckets[i].status == OCCUPIED) {
            TInfo info = h->buckets[i];
            unsigned int hash = hash_function(info.key) % new_n_buckets;
            for(int j = 0; j < new_n_buckets; j++) {
                int idx = (hash + j) % new_n_buckets;
                if(new_buckets[idx].status == EMPTY) {
                    new_buckets[idx] = info;
                    break;
                }
            }
        }
    }
    free(h->buckets);
    h->buckets = new_buckets;
    h->n_buckets = new_n_buckets;
    LOG("  hashtable_expand_and_rehash: expanded to %d buckets\n", new_n_buckets);
    return 1; // success
}

void hashtable_insert(HashTable* h, TKey key, TValue val) {
    if(h->size == h->n_buckets) {
        LOG("Hashtable full (size %d == capacity %d), cannot insert key=%d unless we expand and rehash\n", h->size, h->n_buckets, key);
        int done = hashtable_expand_and_rehash(h);
        if(!done) {
            return;
        }
    }
    TInfo info = { .key = key, .value = val, .status = OCCUPIED };
    unsigned int hash = hashtable_hash(h, key);
    int j = 0;
    int slot = -1;
    for(; j < h->n_buckets; j++) {
        int i = (hash + j) % h->n_buckets;
        if(slot==-1 && (h->buckets[i].status == EMPTY || h->buckets[i].status == DELETED)) {
            slot = i; 
        } else if(equal_key(h->buckets[i].key, key)) {
            LOG("hashtable_insert(%d,%d): found same key at bucket %d, updating value\n", key, val, i);
            h->buckets[i].value = val; // update value
            return;
        }
    }
    if(slot != -1) {
        h->buckets[slot] = info;
        h->size++;
        LOG("  hashtable_insert(%d,%d): inserted at bucket %d (hash = %d)\n", key, val, slot, hash);
    } else {
        LOG("  hashtable_insert(%d,%d): available slot not found\n", key, val);
    }
}

void hashtable_delete(HashTable* ht, TKey key) {
    unsigned int h = hashtable_hash(ht, key);
    for(int i = h; i != (h-1) % ht->n_buckets; i = (i + 1) % ht->n_buckets) {
        if(ht->buckets[i].status == EMPTY) {
            return; // key not found
        }
        if(ht->buckets[i].status == OCCUPIED && equal_key(ht->buckets[i].key, key)) {
            ht->buckets[i].status = DELETED;
            ht->size--;
            return;
        }
    }
}

int hashtable_exists_value(HashTable* h, TValue val) {
    for(int i = 0; i < h->n_buckets; i++) {
        if(h->buckets[i].status == OCCUPIED && equal_value(h->buckets[i].value, val)) {
            return 1;
        }
    }
    return 0;
}

int hashtable_search_keyvalue(HashTable* h, TKey key, TValue val) {
   TValue *v = hashtable_search(h, key);
   return v != NULL && *v == val;
}

TValue *hashtable_search(HashTable* h, TKey key) {
    unsigned int hash = hashtable_hash(h, key);
    for(int i = hash; i != (hash-1) % h->n_buckets; i = (i + 1) % h->n_buckets) {
        if(h->buckets[i].status == EMPTY) {
            return NULL; // key not found
        }
        if(h->buckets[i].status == OCCUPIED && equal_key(h->buckets[i].key, key)) {
            return &h->buckets[i].value;
        }
    }
    return NULL;
}

void hashtable_print(HashTable* h, int include_empty_buckets, char* pre) {
    printf("%s [%d/%d]{\n", pre, h->size, h->n_buckets);
    for(int i = 0; i < h->n_buckets; i++) {
        if(h->buckets[i].status == OCCUPIED) {
            char buf[50];
            info_repr(buf, h->buckets[i]);
            printf("  [%d]: %s\n", i, buf);
        } else if(include_empty_buckets) {
            if(h->buckets[i].status == EMPTY) {
                printf("  [%d]: <EMPTY>\n", i);
            } else if(h->buckets[i].status == DELETED) {
                printf("  [%d]: <DELETED>\n", i);
            }
        }
    }
    printf("}\n");
}

void test_basic_usage() {
    HashTable *h = hashtable_create(2);
    hashtable_insert(h, 4, 77);
    hashtable_insert(h, 44, 66);
    hashtable_print(h, 1, "hashtable after inserting (4,77) and (44,66) =");
    hashtable_insert(h, 85, 99);
    hashtable_insert(h, 175, 55);
    hashtable_print(h, 1, "hashtable after inserting (85,99) and (175,55) =");
    hashtable_insert(h, 5, 123);
    hashtable_print(h, 1, "hashtable after inserting (5,123) =");
    hashtable_print(h, 0, "hashtable =");
    printf("Is key 85 present? %s.\n", hashtable_search(h, 85) ? "yes" : "no");
    printf("Is key 179 present? %s.\n", hashtable_search(h, 179) ? "yes" : "no"); 
    hashtable_delete(h, 4);
    hashtable_print(h, 1, "after removal of key 4 =");
    hashtable_delete(h, 175);
    hashtable_print(h, 1, "after removal of key 175 =");
    hashtable_delete(h, 44);
    hashtable_insert(h, 5, 0);
    hashtable_print(h, 0, "after removal of 44 and insertion of (5,0) =");
}

int main(void) {
    test_basic_usage();
    return 0;
}
