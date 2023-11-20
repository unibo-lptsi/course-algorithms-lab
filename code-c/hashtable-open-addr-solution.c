#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

int equal_key(TKey a, TKey b) {
    return a == b;
}

int equal_value(TValue a, TValue b) {
    return a == b;
}

int info_repr(char *s, TInfo info) {
    return sprintf(s, "%d->%d", info.key, info.value);
}

/* dynamic arrays implementation */

// NB: compile with gcc "-DDEBUG"
#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

static int GROWING_DELTA = 10;
static int SHRINKING_DELTA = 20;
static float GROWING_FACTOR = 2.5;
static float SHRINKING_FACTOR = 3.0;

struct SDArray {
    TInfo** item;
    int capacity;
    int size;
};
typedef struct SDArray DArray;

DArray darray_create(int initial_size) {
    DArray a;
    a.item = (TInfo**) malloc(sizeof(TInfo*) * initial_size);
    assert(initial_size==0 || a.item!=NULL);
    a.capacity = initial_size;
    a.size = initial_size;
    return a;
}

DArray darray_create_capac(int initial_size, int initial_capacity) {
    assert(initial_capacity >= initial_size);
    DArray a;
    a.item = (TInfo**) malloc(sizeof(TInfo*) * initial_capacity);
    assert(initial_size==0 || a.item!=NULL);
    a.capacity = initial_capacity;
    a.size = initial_size;
    return a;
}

TInfo* darray_get(DArray* da, int pos) {
    return (da->item)[pos];
}

void darray_set(DArray* da, int pos, TInfo *value) {
    (da->item)[pos] = value;
}

void darray_print(DArray* da, char* eol) {
    printf("[%d/%d]{", da->size, da->capacity);
    char s[20] = "NULL";
    if(da->size>0){
        int i = 0;
        for(; i < da->size-1; i++) {
            if(da->item[i] != NULL) info_repr(s, *da->item[i]);
            printf("%s, ", s);
        }
        if(da->item[i] != NULL) info_repr(s, *da->item[i]); else strcpy(s, "NULL");
        printf("%s", s);
    }
    printf("}%s", eol);
}

void darray_destroy(DArray* da) {
    free(da->item);
    da->item = NULL;
    da->capacity = 0;
    da->size = 0;
}

void darray_resize_linear(DArray* da, int new_size) {
    if(new_size < 0) return; 
    LOG("[LINEAR] Resize darray to new_size=%d.\n", new_size);
    if(new_size > da->capacity || (da->capacity - new_size) > SHRINKING_DELTA) {
        const int new_capacity = new_size + GROWING_DELTA;
        LOG("[LINEAR] Setting capacity to %d.\n", new_capacity);
        da->item = (TInfo**) realloc(da->item, sizeof(TInfo*) * new_capacity);
        da->capacity = new_capacity;
    }
    da->size = new_size;
}

void darray_resize_geometric(DArray* da, int new_size) {
    if(new_size < 0) return; 
    LOG("[GEOMETRIC] Resize darray to new_size=%d.\n", new_size);
    if(new_size > da->capacity || (da->capacity / new_size) > SHRINKING_FACTOR) {
        const int new_capacity = new_size * GROWING_FACTOR;
        LOG("[GEOMETRIC] Setting capacity to %d.\n", new_capacity);
        da->item = (TInfo**) realloc(da->item, sizeof(TInfo*) * new_capacity);
        da->capacity = new_capacity;
    }
    da->size = new_size;
}

static void (*f_resize)(DArray*,int) = &darray_resize_linear;
void darray_resize(DArray* da, int new_size) {
    f_resize(da, new_size);
}

void darray_append(DArray* da, TInfo* value) {
    int curr_size = da->size; // NB: keep track of current size as resize will move it
    darray_resize(da, curr_size + 1);
    da->item[curr_size] = value;
}

void darray_expand(DArray* da, TInfo** arr, int sz) {
    int curr_size = da->size; // NB: keep track of current size as resize will move it
    darray_resize(da, curr_size + sz);
    for(int i=0; i<sz; i++) {
        darray_set(da, curr_size + i, arr[i]);
    }
}

void darray_insert(DArray* da, int insert_pos, TInfo* value) {
    darray_resize(da, da->size + 1); // make room for an additional element
    // shift elements to free the insert position
    for(int i=da->size-1; i>insert_pos; i--) {
        da->item[i] = da->item[i-1];
    }
    da->item[insert_pos] = value;
}

void darray_assert_equals(DArray* da, TInfo* expected, int expected_len) {
    assert(da->size == expected_len);
    for(int i=0; i<expected_len; i++) {
        assert(equal(*da->item[i], expected[i]));
    }
}

/* Chained Hashtable implementation (closed addressing) */

typedef unsigned int(*hash_function_type)(TKey);
unsigned int hash_int(TKey key);

typedef struct HashTable {
    DArray bucket;
    int nbuckets;
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
int hashtable_search_value(HashTable* h, TValue val);
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
    h->bucket = darray_create(nbuckets);
    if(h->bucket.item == NULL) { free(h); return NULL; }
    h->nbuckets = nbuckets;
    for(int i = 0; i < nbuckets; i++) {
        h->bucket.item[i] = NULL;
    }
    return h;
}

void hashtable_destroy(HashTable* h) {
    if(h == NULL) return;
    darray_destroy(&h->bucket);
    h->nbuckets = 0;
    free(h);
}

unsigned int hashtable_hash(HashTable *h, TKey key) {
    return hash_function(key) % h->nbuckets;
}

void hashtable_insert(HashTable* h, TKey key, TValue val) {
    TInfo* info = (TInfo*) malloc (sizeof(TInfo));
    info-> key = key;
    info->value = val;
    unsigned int hash = hashtable_hash(h, key);
    if(!hashtable_search_keyvalue(h, key, val)) {
        int k = hash;
        for(; h->bucket.item[k] != NULL || k == (hash-1) % h->bucket.capacity; k = (k + 1) % h->bucket.capacity) ;  
        if(h->bucket.item[k] == NULL) h->bucket.item[k] = info;
    }
}

void hashtable_delete(HashTable* ht, TKey key) {
    unsigned int h = hashtable_hash(ht, key);
    int removed = -1;
    for(int i = h; i != (h-1) % ht->nbuckets; i++) {
        if(ht->bucket.item[i] && equal_key((*(ht->bucket.item[i])).key, key)) {
            removed = i;
            free(ht->bucket.item[i]);
            ht->bucket.item[i] = NULL;
            break;
        }
    }
    if(removed >= 0) {
        for(int i = (removed + 1) % ht->nbuckets; 
            i != h && ht->bucket.item[i] && h == hashtable_hash(ht, (*(ht->bucket.item[i])).key); 
            i = (i+1) % ht->nbuckets) {
            ht->bucket.item[i-1] = ht->bucket.item[i];
            ht->bucket.item[i] = NULL;
        }
    }
}

int hashtable_search_value(HashTable* h, TValue val) {
    for(int i = 0; i < h->nbuckets; i++) {
        if(h->bucket.item[i] && equal_value((*(h->bucket.item[i])).value, val)) {
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
    for(int i = hash; i !=  (hash-1) % h->nbuckets; i = (i + 1) % h->nbuckets) {
        if(h->bucket.item[i] && equal_key((*(h->bucket.item[i])).key, key)) {
            return &(*(h->bucket.item[i])).value;
        }
    }
    return NULL;
}

void hashtable_print(HashTable* h, int include_empty_buckets, char* pre) {
    printf("%s {\n", pre);
    for(int i = 0; i < h->nbuckets; i++) {
        if(include_empty_buckets || h->bucket.item[i] != NULL) {
            char s[20] = "NULL";
            if(h->bucket.item[i] != NULL) info_repr(s, *h->bucket.item[i]);
            printf("\t[%d] %s\n", i, s);
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
    hashtable_delete(h, 175);
    hashtable_print(h, 0, "after removal of key 175 =");
    hashtable_delete(h, 44);
    hashtable_insert(h, 5, 0);
    hashtable_print(h, 0, "after removal of 44 and insertion of (5,0) =");

    HashTable *h2 = hashtable_init(10, (TInfo[]) { { 3, 7}, { 5, 8}, {6, 9}, { 33, 10 } }, 4);
    hashtable_print(h2, 0, "h2");
    hashtable_delete(h2, 3);
    hashtable_print(h2, 0, "h2 after removal of key 3");
    printf("Is key 33 present? %s.\n", hashtable_search(h2, 33) ? "yes" : "no");
    return 0;
}
