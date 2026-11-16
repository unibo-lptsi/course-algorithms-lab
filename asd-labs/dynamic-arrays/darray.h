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
 * DArray is an opaque type: this header declares it, but its fields are defined only in darray.c.
 * So, outside darray.c, a DArray is handled only through a pointer (DArray*), created by darray_create,
 * darray_create_capac or darray_init and freed by darray_destroy, and its fields are read only through
 * functions (e.g., darray_size). Why: a resize may move the elements (realloc) and free the old block, to which
 * a copy of the struct (DArray b = *a;) would still point. With an opaque type, such a copy does not compile,
 * while a copy of the pointer (DArray* b = a;) refers to the same array, and so it sees all its changes.
 * NB: sizes are int rather than size_t (the idiomatic type for sizes) to keep the code simple:
 * e.g., with an unsigned i, a loop like for(i=size-1; i>=0; i--) would never end.
 */
typedef struct SDArray DArray;

/**
 * Preconditions (e.g., 0 <= pos < size) must be guaranteed by the caller: violating one is a bug of the caller,
 * so the function stops the program at once (abort), with a message saying which precondition was violated.
 */

// Creation and destruction (precondition: 0 <= initial_size <= initial_capacity)
DArray* darray_create(int initial_size);
DArray* darray_create_capac(int initial_size, int initial_capacity);
DArray* darray_init(int initial_size, TInfo value);
// Frees the array (the elements and the DArray itself): afterwards, neither da nor its copies may be used
void darray_destroy(DArray* da);

// Size and capacity (invariant: 0 <= size <= capacity)
int darray_size(const DArray* da);
int darray_capacity(const DArray* da);

// Access to the elements (precondition: 0 <= pos < size)
TInfo darray_get(const DArray* da, int pos);
void darray_set(DArray* da, int pos, TInfo value);
// Prints the array as [size/capacity]{a_0, ..., a_{n-1}}, followed by eol, on the given stream
void darray_print(FILE* out, const DArray* da, const char* eol);

// Changing the size (and possibly the capacity)
void darray_realloc(DArray* da, int new_capacity);           // precondition: new_capacity >= size
void darray_resize(DArray* da, int new_size);                // precondition: new_size >= 0
void darray_append(DArray* da, TInfo value);
// Appends arr[0..sz-1] (precondition: sz >= 0; arr may be NULL only if sz == 0)
void darray_expand(DArray* da, const TInfo* arr, int sz);
void darray_insert(DArray* da, int insert_pos, TInfo value); // precondition: 0 <= insert_pos <= size
TInfo darray_remove(DArray* da, int remove_pos);             // precondition: 0 <= remove_pos < size

/**
 * Resize policy used by darray_resize, with its parameters. The setters check these constraints (why are they needed?):
 * - linear:    1 <= growing_delta < shrinking_delta
 * - geometric: 1 < growing_factor < shrinking_factor
 * NB: the policy is global: it applies to all the arrays, including the existing ones.
 */
void darray_set_linear_policy(int growing_delta, int shrinking_delta);
void darray_set_geometric_policy(double growing_factor, double shrinking_factor);

#endif // DARRAY_H
