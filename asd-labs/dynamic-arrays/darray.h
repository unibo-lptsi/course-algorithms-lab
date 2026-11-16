/**
 * Module darray: dynamic arrays (interface).
 *
 * This header declares what the users of the module (e.g., main.c, darray_test.c) may use: the types and the
 * public functions. Their implementation is in darray.c, together with the private details (declared static
 * there), e.g. the parameters of the resize policies.
 */
#ifndef DARRAY_H // include guard: if the header is included twice, the second copy is skipped
#define DARRAY_H

#include <stdio.h> // FILE

typedef int TInfo;
#define TINFO_FMT "%d" // printf format of TInfo: change it together with the typedef

// Prints items[0..n-1] as {a_0, ..., a_{n-1}} on the given stream (e.g., stdout or stderr)
void tinfo_print_array(FILE* out, const TInfo* items, int n);

/**
 * Invariant: 0 <= size <= capacity; item[0..size-1] are the elements, item[size..capacity-1] is free space.
 * NB: always pass a DArray by pointer. A copy (DArray b = a;) shares the buffer of a, which any resize of a
 * may move or free (realloc): pointers into item[] are invalidated by resizes.
 * NB: sizes are int rather than size_t (the idiomatic type for sizes) to keep the code simple:
 * e.g., with an unsigned i, a loop like for(i=size-1; i>=0; i--) would never end.
 */
struct SDArray {
    TInfo* item;
    int capacity;
    int size;
};
typedef struct SDArray DArray;

// Creation and destruction
DArray darray_create(int initial_size);
DArray darray_create_capac(int initial_size, int initial_capacity);
DArray darray_init(int initial_size, TInfo value);
void darray_destroy(DArray* da);

// Access to the elements (precondition: 0 <= pos < size)
TInfo darray_get(const DArray* da, int pos);
void darray_set(DArray* da, int pos, TInfo value);
void darray_print(const DArray* da, const char* eol);

// Changing the size (and possibly the capacity)
void darray_realloc(DArray* da, int new_capacity);
void darray_resize(DArray* da, int new_size);
void darray_append(DArray* da, TInfo value);
void darray_expand(DArray* da, const TInfo* arr, int sz);
void darray_insert(DArray* da, int insert_pos, TInfo value);
TInfo darray_remove(DArray* da, int remove_pos);

/**
 * Resize policy used by darray_resize, with its parameters. The setters check these constraints (why are they needed?):
 * - linear:    1 <= growing_delta < shrinking_delta
 * - geometric: 1 < growing_factor < shrinking_factor
 * NB: the policy is global: it applies to all the arrays, including the existing ones.
 */
void darray_set_linear_policy(int growing_delta, int shrinking_delta);
void darray_set_geometric_policy(double growing_factor, double shrinking_factor);

#endif // DARRAY_H
