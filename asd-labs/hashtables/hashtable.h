/**
 * Module hashtable: hash tables with chaining (closed addressing) (interface).
 *
 * A table has nbuckets buckets (m, in the slides); bucket[i] is the chain (a list, see list.h) of the entries
 * whose key hashes to i, i.e. hashtable_hash(h, key) == i.
 * A table is either fixed (created by hashtable_create): its number of buckets never changes, so it can hold more
 * entries than buckets, in longer and longer chains; or dynamic (created by hashtable_create_dynamic): it expands,
 * i.e. it gets more buckets, when an insertion makes its load factor exceed max_load_factor (exercise 2).
 * Invariants: each entry is in the chain of the bucket its key hashes to; no key occurs twice in the table;
 * size is the number of entries; in a dynamic table, size <= max_load_factor * nbuckets (unless an expansion
 * fails for lack of memory).
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
    double max_load_factor; // 0 in a fixed table; > 0 in a dynamic one, the maximum load factor (see above)
} HashTable;

/**
 * Hash functions: map a key to a bucket index in [0, m-1], where m is the number of buckets.
 * Each table keeps the hash function in use when it is created (initially hash_int): hashtable_set_hash_function
 * changes the one of the tables created afterwards, not the existing ones (whose entries are in the buckets given
 * by their function: with another function, they would become unreachable).
 */
unsigned int hash_int(TKey key, int m);
void hashtable_set_hash_function(hash_function_type f);

// Creation of a fixed table and of a dynamic one (preconditions: nbuckets > 0, max_load_factor > 0), and destruction
// (which frees the nodes of the chains, the buckets and the HashTable itself)
HashTable *hashtable_create(int nbuckets);
HashTable *hashtable_create_dynamic(int nbuckets, double max_load_factor);
void hashtable_destroy(HashTable* h);

// The bucket index of key, and its chain
unsigned int hashtable_hash(HashTable *h, TKey key);
list* hashtable_list(HashTable *h, TKey key);

// Exercise 1: inserts the entry key->val, at the head of the chain of its bucket; if key is already present, it
// updates its value instead.
// NB: unlike CHAINED-HASH-INSERT (CLRS), which assumes that key is not present and takes O(1) time, it searches
// the chain first, so it takes O(1 + n/m) time on average (as a search): this keeps the invariant "no key twice".
// In a dynamic table, if the new entry makes the load factor exceed max_load_factor, it expands the table
// (exercise 2: about twice the buckets, so that an insertion takes O(1) amortized time for the expansions)
void hashtable_insert(HashTable* h, TKey key, TValue val);
// Deletes the entry with the given key, if present
void hashtable_delete(HashTable* h, TKey key);

// Exercise 1: returns a pointer to the value of key, i.e. to the value field of the node holding key (so, the caller
// can also change the value through it; it is valid until key is deleted), or NULL if key is not present
TValue *hashtable_search(HashTable* h, TKey key);
// Returns 1 iff some key has the value val, 0 otherwise
int hashtable_search_value(HashTable* h, TValue val);

// Prints on stdout pre followed by the chains of the buckets (of the empty ones too, if include_empty_buckets)
void hashtable_print(HashTable* h, int include_empty_buckets, const char *pre);

/**
 * Exercise 1 (see README.md), besides hashtable_insert and hashtable_search above
 */
// Inserts entries[0..nentries-1] into h, in this order, with hashtable_insert: so, if a key occurs more than once,
// the last value wins (preconditions: nentries >= 0; entries may be NULL only if nentries == 0)
void hashtable_add_many(HashTable* h, const TInfo* entries, int nentries);
// Creates a table with nbuckets buckets holding the entries entries[0..nentries-1] (inserted in this order, as by
// hashtable_add_many: if a key occurs more than once, the last value wins)
HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries);
// Returns a new table holding the entries of both h1 and h2 (for the keys in both, the value in h2 wins);
// h1 and h2 are left unchanged, and the new table shares no node with them
HashTable *hashtable_merge(HashTable* h1, HashTable *h2);

/**
 * Exercise 2 (see README.md): expansion and rehashing (besides the expansion in hashtable_insert)
 */
// Moves the entries of h into new_nbuckets new buckets, each into the one its key hashes to with new_nbuckets
// buckets (rehashing), by relinking their nodes: none is allocated or freed, so the pointers returned by
// hashtable_search stay valid. If there is no memory for the new buckets, h is left unchanged.
// Precondition: new_nbuckets > 0
void hashtable_rehash(HashTable* h, int new_nbuckets);

#endif // HASHTABLE_H
