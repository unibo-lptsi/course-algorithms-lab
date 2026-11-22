/**
 * Module hashtable: hash tables with chaining (closed addressing) (interface).
 *
 * A table has nbuckets buckets (m, in the slides); bucket[i] is the chain (a list, see list.h) of the entries
 * whose key hashes to i, i.e. hashtable_hash(h, key) == i.
 * Invariants: each entry is in the chain of the bucket its key hashes to; no key occurs twice in the table;
 * size is the number of entries.
 * The operations on the entries (insert, delete, search) accept a NULL table, treated as an empty table.
 */
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "list.h"
#include "tinfo.h"

/* Chained Hashtable implementation (closed addressing) */

typedef unsigned int(*hash_function_type)(TKey, int m); // hash functions: see below

// NB: the fields are public, unlike those of DArray (opaque, in the dynamic-arrays lab), so that the tests can check
// the buckets ("white box"); the other modules should only read them, and change the table through the functions below
typedef struct HashTable {
    list** bucket;
    int nbuckets;
    int size; // the number of entries (n): the load factor is size / nbuckets
    hash_function_type hash; // the hash function of this table, fixed when it is created
} HashTable;

/**
 * Hash functions: map a key to a bucket index in [0, m-1], where m is the number of buckets.
 * Each table keeps the hash function in use when it is created (initially hash_int): hashtable_set_hash_function
 * changes the one of the tables created afterwards, not the existing ones (whose entries are in the buckets given
 * by their function: with another function, they would become unreachable).
 */
unsigned int hash_int(TKey key, int m);
void hashtable_set_hash_function(hash_function_type f);

// Creation (precondition: nbuckets > 0) and destruction (which frees the nodes of the chains, the buckets and the
// HashTable itself)
HashTable *hashtable_create(int nbuckets);
void hashtable_destroy(HashTable* h);

// The bucket index of key, and its chain
unsigned int hashtable_hash(HashTable *h, TKey key);
list* hashtable_list(HashTable *h, TKey key);

// Inserts the entry key->val; if key is already present, it updates its value.
// NB: unlike CHAINED-HASH-INSERT (CLRS), which assumes that key is not present and takes O(1) time, it searches
// the chain first, so it takes O(1 + n/m) time on average (as a search): this keeps the invariant "no key twice"
void hashtable_insert(HashTable* h, TKey key, TValue val);
// Deletes the entry with the given key, if present
void hashtable_delete(HashTable* h, TKey key);
// Deletes the entry key->val, if present
void hashtable_delete_value(HashTable* h, TKey key, TValue val);

// Returns a pointer to the value of key (valid until key is deleted), or NULL if key is not present
TValue *hashtable_search(HashTable* h, TKey key);
// Returns 1 iff some key has the value val, 0 otherwise
int hashtable_search_value(HashTable* h, TValue val);
// Returns 1 iff the table holds the entry key->val, 0 otherwise
int hashtable_search_keyvalue(HashTable* h, TKey key, TValue val);
// Returns the node holding key, or NULL if key is not present
list *hashtable_search_list_item(HashTable* h, TKey key);

// Prints on stdout pre followed by the chains of the buckets (of the empty ones too, if include_empty_buckets)
void hashtable_print(HashTable* h, int include_empty_buckets, const char *pre);

/**
 * Exercises (see README.md)
 */
// Creates a table with nbuckets buckets holding the entries entries[0..nentries-1] (inserted in this order: if a key
// occurs more than once, the last value wins)
HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries);
// Returns a new table holding the entries of both h1 and h2 (for the keys in both, the value in h2 wins);
// h1 and h2 are left unchanged, and the new table shares no node with them
HashTable *hashtable_merge(HashTable* h1, HashTable *h2);

#endif // HASHTABLE_H
