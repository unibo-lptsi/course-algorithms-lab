#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * TODO: implementare le seguenti funzioni:
 * - darray_append(DArray* da, TInfo value)
 * - darray_insert(DArray* da, int insert_pos, TInfo value)
 * - darray_assert_equals(DArray* da, TInfo* expected, int expected_len)
 * - DArray darray_init(int initial_size, TInfo value)
 */

// NB: compile with gcc "-DDEBUG"
#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stdout, fmt, __VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

static int GROWING_DELTA = 10;
static int SHRINKING_DELTA = 20;
static float GROWING_FACTOR = 2.5;
static float SHRINKING_FACTOR = 3.0;

typedef int TInfo;

struct SDArray {
    TInfo* item;
    int capacity;
    int size;
};
typedef struct SDArray DArray;

void darray_resize(DArray* da, int new_size);
DArray darray_create(int initial_size);
DArray darray_create_capac(int initial_size, int initial_capacity);
DArray darray_init(int initial_size, TInfo value);
void darray_set(DArray* da, int pos, TInfo value);
void darray_print(DArray* da, char* eol);
void darray_destroy(DArray* da);
void darray_realloc(DArray* da, int new_capacity);
void darray_resize_linear(DArray* da, int new_size);
void darray_resize_geometric(DArray* da, int new_size);
void darray_resize(DArray* da, int new_size);
void darray_append(DArray* da, TInfo elem);
void darray_expand(DArray* da, TInfo* arr, int sz);
void darray_insert(DArray* da, int insert_pos, TInfo value);
void darray_assert_equals(DArray* da, TInfo* expected, int expected_len);


void darray_append(DArray* da, TInfo value) {
    // TODO
    fprintf(stderr, "darray_append: TO BE IMPLEMENTED\n");
}

void darray_insert(DArray* da, int insert_pos, TInfo value) {
    // TODO
    fprintf(stderr, "darray_insert: TO BE IMPLEMENTED\n");
}

void darray_assert_equals(DArray* da, TInfo* expected, int expected_len) {
    // TODO
    fprintf(stderr, "darray_assert_equals: TO BE IMPLEMENTED\n");
}

DArray darray_init(int initial_size, TInfo value) {
    // TODO
    fprintf(stderr, "darray_init: TO BE IMPLEMENTED\n");    
}

static void (*f_resize)(DArray*,int) = &darray_resize_linear;

DArray darray_create(int initial_size) {
    return darray_create_capac(initial_size, initial_size);
}

DArray darray_create_capac(int initial_size, int initial_capacity) {
    if(initial_capacity < initial_size) {
        fprintf(stderr, "WARNING: initial_capacity %d is less than initial_size %d; adjusting initial_capacity to initial_size.\n", initial_capacity, initial_size);
        initial_capacity = initial_size;
    }

    DArray a;
    a.item = (TInfo*) malloc(sizeof(TInfo) * initial_capacity);
    if(a.item == NULL) {
        fprintf(stderr, "ERROR: malloc failed; exiting.\n");
        exit(-1);
    }
    a.capacity = initial_capacity;
    a.size = initial_size;
    return a;
}

void darray_set(DArray* da, int pos, TInfo value) {
    int curr_size = da->size;
    // one approach would be to conditionally set the value at pos iff pos < size
    // another approach would be to expand the size (but then the user should be aware that a O(n) cost may apply)
    if(da->size <= pos) {
        darray_resize(da, pos+1);
        // for(int i=curr_size; i<pos+1; i++) darray_set(da, i, 0);
    }
    (da->item)[pos] = value;
}

void darray_print(DArray* da, char* eol) {
    printf("[%d/%d]{", da->size, da->capacity);
    if(da->size>0){
        int i = 0;
        for(; i < da->size-1; i++) {
            printf("%d, ", da->item[i]);
        }
        printf("%d", da->item[i]);
    }
    printf("}%s", eol);
}

void darray_destroy(DArray* da) {
    free(da->item);
    da->item = NULL;
    da->capacity = 0;
    da->size = 0;
}

void darray_realloc(DArray* da, int new_capacity) {
    if(new_capacity < da->size) {
        fprintf(stderr, "WARNING: requested capacity %d is less than current size %d; realloc skipped.\n", new_capacity, da->size);
        return;
    }
    LOG("[LOG] Requested reallocating and setting new_capacity=%d.\n", new_capacity);
    da->item = (TInfo*) realloc(da->item, sizeof(TInfo) * new_capacity);
    if(da->item == NULL) {
        fprintf(stderr, "ERROR: realloc failed; exiting.\n");
        exit(-1);
    }
    da->capacity = new_capacity;
}

void darray_resize_linear(DArray* da, int new_size) {
    if(new_size < 0) return; 
    LOG("[LINEAR] Resize darray to new_size=%d.\n", new_size);
    if(new_size > da->capacity || (da->capacity - new_size) > SHRINKING_DELTA) {
        const int new_capacity = new_size + GROWING_DELTA;
        LOG("[LINEAR] Setting capacity to %d.\n", new_capacity);
        darray_realloc(da, new_capacity);
    }
    da->size = new_size;
}

void darray_resize_geometric(DArray* da, int new_size) {
    if(new_size < 0) return; 
    LOG("[GEOMETRIC] Resize darray to new_size=%d.\n", new_size);
    if(new_size > da->capacity || (da->capacity / new_size) > SHRINKING_FACTOR) {
        const int new_capacity = new_size * GROWING_FACTOR;
        LOG("[GEOMETRIC] Setting capacity to %d.\n", new_capacity);
        darray_realloc(da, new_capacity);
    }
    da->size = new_size;
}

void darray_resize(DArray* da, int new_size) {
    f_resize(da, new_size);
}


void darray_expand(DArray* da, TInfo* arr, int sz) {
    int curr_size = da->size; // NB: keep track of current size as resize will move it
    darray_resize(da, curr_size + sz);
    for(int i=0; i<sz; i++) {
        darray_set(da, curr_size + i, arr[i]);
    }
}

void test() {
    printf("*** TEST ***\n");
    DArray da = darray_create(5);
    darray_print(&da,"\n");
    for(int i=0; i<5; i++) {
        printf("setting d[%d] to %d\n", i, i+1);
        darray_set(&da, i, i+1);
    }
    darray_assert_equals(&da, (int[]){1,2,3,4,5}, 5);
    darray_print(&da,"\n");
    darray_realloc(&da, da.capacity + 3);
    darray_print(&da,"\n");
    printf("appending 88\n");
    darray_append(&da, 88);
    darray_assert_equals(&da, (int[]){1,2,3,4,5,88}, 6);
    darray_print(&da,"\n");
    printf("doing some resizes (first to 17, then back to 3)\n");
    darray_resize(&da, 17);
    darray_resize(&da, 3);
    darray_assert_equals(&da, (int[]){1,2,3}, 3);
    printf("expanding with 11 new elements 0, ..., 10\n");
    darray_expand(&da, (TInfo[]){0,1,2,3,4,5,6,7,8,9,10}, 11);
    darray_print(&da,"\n");
    darray_assert_equals(&da, (int[]){1,2,3,0,1,2,3,4,5,6,7,8,9,10}, 14);
    printf("inserting 55 at position 2\n");
    darray_insert(&da, 2, 55);
    //darray_resize(&da, 100);
    printf("setting position 80 to 888\n");
    darray_set(&da, 80, 888);
    darray_assert_equals(&da, (int[]){1,2,55,3,0,1,2,3,4,5,6,7,8,9,10}, 15);
    darray_print(&da,"\n\n");
    printf("Destroying the array\n");
    darray_destroy(&da);
    printf("Creating and initializing an array of size 7 with all values set to 42\n");
    da = darray_init(7, 42);
    darray_print(&da,"\n");
    darray_assert_equals(&da, (int[]){42,42,42,42,42,42,42}, 7);
    darray_destroy(&da);
    printf("*** END TEST ***\n");
}

int main() {
    f_resize = &darray_resize_linear;
    GROWING_DELTA = 10;
    SHRINKING_DELTA = 12;
    test();

    f_resize = &darray_resize_geometric;
    GROWING_FACTOR = 4.5;
    SHRINKING_FACTOR = 8.5;
    test();
}