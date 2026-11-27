/**
 * Module tinfo: the information pieces stored in the hash table, i.e. key-value pairs (interface).
 *
 * The key identifies an entry: two TInfo with the same key are equal (tinfo_equal), whatever their values.
 * The other modules (list, hashtable, ...) include this header to know the types TKey, TValue and TInfo.
 */
#ifndef TINFO_H // include guard: if the header is included twice, the second copy is skipped
#define TINFO_H

typedef int TKey;
typedef int TValue;
// printf formats of TKey and TValue: change them together with the typedefs
#define TKEY_FMT "%d"
#define TVALUE_FMT "%d"

/* Information pieces are key-value pairs */
typedef struct TInfo {
    TKey key;
    TValue value;
} TInfo;

// Returns 1 iff a and b have the same key, 0 otherwise (NB: equality only based on keys)
int tinfo_equal(TInfo a, TInfo b);

// Prints on stdout the representation "key->value" of info (as printf, of which it returns the result)
int tinfo_print(TInfo info);

#endif // TINFO_H
