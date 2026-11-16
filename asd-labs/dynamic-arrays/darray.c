/**
 * Module darray: dynamic arrays (implementation). The interface is in darray.h.
 */
#include "darray.h" // first, so that the compiler checks the header does not depend on previous includes

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Definition of the opaque type DArray (declared in darray.h): only the functions of this file see its fields.
 * Invariant: 0 <= size <= capacity; item[0..size-1] are the elements, item[size..capacity-1] is free space.
 * NB: a resize may move the elements (realloc), so it invalidates the pointers into item[].
 */
struct SDArray {
    TInfo* item;
    int capacity;
    int size;
};

/**
 * TODO: implementare le seguenti funzioni:
 * - void darray_append(DArray* da, TInfo value)
 * - void darray_insert(DArray* da, int insert_pos, TInfo value)
 * - TInfo darray_remove(DArray* da, int remove_pos)
 * - DArray* darray_init(int initial_size, TInfo value)
 */

// Prints "<function name>: TO BE IMPLEMENTED"
// fflush(stdout) prints the pending output first, to keep the messages in order
#define NOT_IMPLEMENTED() do { \
        fflush(stdout); \
        fprintf(stderr, "    %s: TO BE IMPLEMENTED\n", __func__); \
    } while(0)

// NB: compile darray.c with gcc "-DDEBUG" to print log messages (on stderr)
#ifdef DEBUG
#define LOG_ENABLED 1
#else
#define LOG_ENABLED 0
#endif
// NB: "if(0)" is optimised away, but the compiler still checks the arguments;
// "do { ... } while(0)" makes the macro a single statement
#define LOG(...) do { if(LOG_ENABLED) fprintf(stderr, __VA_ARGS__); } while(0)

/**
 * REQUIRE(cond) checks a precondition of a public function, i.e. a condition that the caller must guarantee
 * (e.g., a valid position): if it is false, it reports where and which one, then stops the program (abort).
 * Why not assert: compiling with -DNDEBUG (as usual for "release" builds) removes all the asserts, and an
 * unchecked violation would silently corrupt memory (e.g., writing outside the allocated block).
 * assert is for the internal checks instead: conditions that only a bug of this file could break.
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

#define MIN_CAPACITY 1 // never allocate 0 bytes: malloc(0) and realloc(p, 0) may return NULL

/**
 * Parameters of the resize policies. They are static, i.e. private to this file: the other modules can change
 * them only through darray_set_linear_policy() and darray_set_geometric_policy(), which check their constraints.
 */
static int GROWING_DELTA = 10;
static int SHRINKING_DELTA = 20;
static double GROWING_FACTOR = 2.5;
static double SHRINKING_FACTOR = 3.0;

// The resize policies: static (private) functions, used only through darray_resize
static void darray_resize_linear(DArray* da, int new_size);
static void darray_resize_geometric(DArray* da, int new_size);

// The resize policy in use (see darray_resize)
static void (*f_resize)(DArray*, int) = &darray_resize_linear;


void darray_append(DArray* da, TInfo value) {
    // TODO
    NOT_IMPLEMENTED();
}

void darray_insert(DArray* da, int insert_pos, TInfo value) {
    // TODO (precondition: 0 <= insert_pos <= size, to check with REQUIRE)
    // step 1: resize to give room for the additional element (size+1)
    // step 2: shift the elements from the insert position to the right
    // step 3: set item at the insert position
    NOT_IMPLEMENTED();
}

TInfo darray_remove(DArray* da, int remove_pos) {
    // TODO (precondition: 0 <= remove_pos < size, to check with REQUIRE; returns the removed element)
    NOT_IMPLEMENTED();
    return (TInfo){0}; // dummy value
}

DArray* darray_init(int initial_size, TInfo value) {
    // TODO
    NOT_IMPLEMENTED();
    return darray_create(0); // dummy value (an empty array), so that the caller can still use and destroy it
}

DArray* darray_create(int initial_size) {
    return darray_create_capac(initial_size, initial_size);
}

DArray* darray_create_capac(int initial_size, int initial_capacity) {
    // NB: a capacity less than the size is a bug of the caller too: better to stop than to fix it silently
    REQUIRE(0 <= initial_size && initial_size <= initial_capacity);
    if(initial_capacity < MIN_CAPACITY) {
        initial_capacity = MIN_CAPACITY;
    }

    // NB: the DArray itself is allocated here, on the heap: the users of the module could not allocate it,
    // as they do not know its fields, nor its size (sizeof(DArray)), DArray being opaque
    DArray* da = malloc(sizeof(DArray));
    if(da == NULL) {
        fprintf(stderr, "ERROR: malloc failed; exiting.\n");
        exit(EXIT_FAILURE);
    }
    // NB: calloc zero-initialises the elements (with malloc, their value would be indeterminate)
    da->item = calloc((size_t)initial_capacity, sizeof(TInfo));
    if(da->item == NULL) {
        fprintf(stderr, "ERROR: calloc failed; exiting.\n");
        free(da);
        exit(EXIT_FAILURE);
    }
    da->capacity = initial_capacity;
    da->size = initial_size;
    return da;
}

int darray_size(const DArray* da) {
    return da->size;
}

int darray_capacity(const DArray* da) {
    return da->capacity;
}

TInfo darray_get(const DArray* da, int pos) {
    REQUIRE(0 <= pos && pos < da->size);
    return da->item[pos];
}

void darray_set(DArray* da, int pos, TInfo value) {
    // Precondition: 0 <= pos < size. Violating it is a bug of the caller, hence REQUIRE, which stops the program:
    // just printing a warning and going on would make the bug easy to miss.
    // Alternatives: returning an error code; or expanding the size (but then the user should be aware
    // that a O(n) cost may apply, and the name should say it, e.g. darray_set_or_expand):
    /**
    if(da->size <= pos) {
        darray_resize(da, pos+1);
    }
    */
    REQUIRE(0 <= pos && pos < da->size);
    da->item[pos] = value;
}

void tinfo_print_array(FILE* out, const TInfo* items, int n) {
    fprintf(out, "{");
    for(int i = 0; i < n; i++) {
        if(i > 0) fprintf(out, ", ");
        fprintf(out, TINFO_FMT, items[i]);
    }
    fprintf(out, "}");
}

void darray_print(FILE* out, const DArray* da, const char* eol) {
    fprintf(out, "[%d/%d]", da->size, da->capacity);
    tinfo_print_array(out, da->item, da->size);
    fprintf(out, "%s", eol);
}

void darray_destroy(DArray* da) {
    if(da == NULL) {
        return; // as free(NULL), destroying no array does nothing
    }
    free(da->item); // NB: before free(da), after which da->item could no longer be read
    free(da);
}

void darray_realloc(DArray* da, int new_capacity) {
    REQUIRE(new_capacity >= da->size); // keep the invariant size <= capacity
    if(new_capacity < MIN_CAPACITY) {
        new_capacity = MIN_CAPACITY;
    }
    LOG("[LOG] Requested reallocating and setting new_capacity=%d.\n", new_capacity);
    // NB: not "da->item = realloc(da->item, ...)": when realloc fails, it returns NULL but the old block
    // is still allocated, and we would lose the only pointer to it
    TInfo* new_item = realloc(da->item, sizeof(TInfo) * (size_t)new_capacity);
    if(new_item == NULL) {
        fprintf(stderr, "ERROR: realloc failed; exiting.\n");
        darray_destroy(da);
        exit(EXIT_FAILURE);
    }
    da->item = new_item;
    da->capacity = new_capacity;
}

static void darray_resize_linear(DArray* da, int new_size) {
    assert(new_size >= 0); // internal check: guaranteed by darray_resize, the only caller
    LOG("[LINEAR] Resize darray to new_size=%d.\n", new_size);
    da->size = new_size; // NB: before darray_realloc, which checks new_capacity >= size
    if(new_size > da->capacity || (da->capacity - new_size) > SHRINKING_DELTA) {
        const int new_capacity = new_size + GROWING_DELTA;
        LOG("[LINEAR] Setting capacity to %d.\n", new_capacity);
        darray_realloc(da, new_capacity);
    }
}

static void darray_resize_geometric(DArray* da, int new_size) {
    assert(new_size >= 0); // internal check: guaranteed by darray_resize, the only caller
    LOG("[GEOMETRIC] Resize darray to new_size=%d.\n", new_size);
    da->size = new_size; // NB: before darray_realloc, which checks new_capacity >= size
    // NB: "capacity > SHRINKING_FACTOR * new_size" rather than "capacity / new_size > SHRINKING_FACTOR":
    // no integer division (which truncates), and no division by zero when new_size == 0
    if(new_size > da->capacity || da->capacity > SHRINKING_FACTOR * new_size) {
        int new_capacity = (int)(new_size * GROWING_FACTOR);
        if(new_capacity <= new_size) {
            new_capacity = new_size + 1; // the truncation may leave no free slot (small sizes, factors near 1)
        }
        if(new_capacity != da->capacity) {
            LOG("[GEOMETRIC] Setting capacity to %d.\n", new_capacity);
            darray_realloc(da, new_capacity);
        }
    }
}

void darray_resize(DArray* da, int new_size) {
    REQUIRE(new_size >= 0);
    const int old_size = da->size;
    f_resize(da, new_size);
    // the new elements (if any) are zero-initialised, so that item[0..size-1] never holds garbage
    for(int i = old_size; i < new_size; i++) {
        da->item[i] = (TInfo){0};
    }
    // internal check: the resize policies must keep the invariant
    assert(da->size == new_size && da->size <= da->capacity);
}

void darray_set_linear_policy(int growing_delta, int shrinking_delta) {
    REQUIRE(1 <= growing_delta && growing_delta < shrinking_delta);
    GROWING_DELTA = growing_delta;
    SHRINKING_DELTA = shrinking_delta;
    f_resize = &darray_resize_linear;
}

void darray_set_geometric_policy(double growing_factor, double shrinking_factor) {
    REQUIRE(1 < growing_factor && growing_factor < shrinking_factor);
    GROWING_FACTOR = growing_factor;
    SHRINKING_FACTOR = shrinking_factor;
    f_resize = &darray_resize_geometric;
}

// Precondition: arr does not point into da->item, which the resize may free (e.g., not darray_expand(da, da->item, ...));
// only the functions of this file could break it, as da->item is not accessible elsewhere
void darray_expand(DArray* da, const TInfo* arr, int sz) {
    REQUIRE(sz >= 0);
    REQUIRE(arr != NULL || sz == 0);
    int curr_size = da->size; // NB: keep track of current size as resize will move it
    darray_resize(da, curr_size + sz);
    // NB: memcpy requires valid pointers even to copy 0 bytes (and arr may be NULL when sz == 0)
    if(sz > 0) {
        memcpy(&da->item[curr_size], arr, sizeof(TInfo) * (size_t)sz);
    }
}
