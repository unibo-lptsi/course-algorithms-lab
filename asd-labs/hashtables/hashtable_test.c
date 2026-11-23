/**
 * Module hashtable_test: tests of the hashtable module (implementation).
 *
 * Each test_* function tests an operation (or a property) of the hash tables, with the harness of test_harness.h.
 * Besides its generic checks (CHECK, CHECK_INT), the tests use the following checks, defined here:
 * - CHECK_TABLE(h, expected, n): h holds exactly the entries expected[0..n-1] (in any order), and the invariants
 *   of hashtable.h hold (each entry is in the bucket its key hashes to, no key occurs twice, its size is n)
 * - CHECK_FOUND(h, key, value), CHECK_NOT_FOUND(h, key): the result of hashtable_search(h, key)
 * - CHECK_CHAIN(h, i, expected, n): the chain of bucket i holds exactly expected[0..n-1], in this order
 * NB: these checks read the buckets (h->bucket, h->nbuckets): they are "white box" checks, which test how the
 * entries are stored, besides what the operations return.
 *
 * In the report, a table is shown as [n/m]{[i](chain of bucket i) ...}, with n entries and m buckets (so, n/m is
 * the load factor), listing only the non-empty buckets: e.g., [3/10]{[4](44->66 4->77) [5](85->99)}.
 */
#include "hashtable_test.h"

#include <stdio.h>

#include "hashtable.h"
#include "test_harness.h"

/* Representation of entries and tables in the report */

static void append_entry(StrBuf* sb, TInfo e) {
    strbuf_printf(sb, TKEY_FMT "->" TVALUE_FMT, e.key, e.value);
}

// Appends entries[0..n-1] as {k1->v1, k2->v2, ...}
static void append_entries(StrBuf* sb, const TInfo* entries, int n) {
    strbuf_printf(sb, "{");
    for(int i = 0; i < n; i++) {
        strbuf_printf(sb, i == 0 ? "" : ", ");
        append_entry(sb, entries[i]);
    }
    strbuf_printf(sb, "}");
}

// Appends entries[0..n-1] as the chain (k1->v1 k2->v2 ...), as list_print
static void append_chain_of_entries(StrBuf* sb, const TInfo* entries, int n) {
    strbuf_printf(sb, "(");
    for(int i = 0; i < n; i++) {
        strbuf_printf(sb, i == 0 ? "" : " ");
        append_entry(sb, entries[i]);
    }
    strbuf_printf(sb, ")");
}

// Appends the chain l as (k1->v1 k2->v2 ...), as list_print
static void append_chain(StrBuf* sb, const list* l) {
    strbuf_printf(sb, "(");
    for(const list* node = l; node != NULL; node = node->next) {
        strbuf_printf(sb, node == l ? "" : " ");
        append_entry(sb, node->val);
    }
    strbuf_printf(sb, ")");
}

// Returns the number of entries of h (0 if h is NULL)
static int count_entries(HashTable* h) {
    int n = 0;
    for(int i = 0; h != NULL && i < h->nbuckets; i++) {
        for(list* l = h->bucket[i]; l != NULL; l = l->next) {
            n++;
        }
    }
    return n;
}

// Appends h as [n/m]{[i](chain of bucket i) ...} (only the non-empty buckets), or NULL
static void append_table(StrBuf* sb, HashTable* h) {
    if(h == NULL) {
        strbuf_printf(sb, "NULL");
        return;
    }
    strbuf_printf(sb, "[%d/%d]{", count_entries(h), h->nbuckets);
    int first = 1;
    for(int i = 0; i < h->nbuckets; i++) {
        if(h->bucket[i] != NULL) {
            strbuf_printf(sb, "%s[%d]", first ? "" : " ", i);
            append_chain(sb, h->bucket[i]);
            first = 0;
        }
    }
    strbuf_printf(sb, "}");
}

/* Trace */

// Adds to the trace the line: name = h
static void trace_table(const char* name, HashTable* h) {
    if(trace_on()) {
        StrBuf sb = STRBUF_INIT;
        append_table(&sb, h);
        trace("%s = %s", name, strbuf_str(&sb));
        strbuf_free(&sb);
    }
}

// Adds to the trace the line: [ACT] action entries[0..n-1]
static void trace_action_with_entries(const char* action, const TInfo* entries, int n) {
    if(trace_on()) {
        StrBuf sb = STRBUF_INIT;
        append_entries(&sb, entries, n);
        trace("[ACT] %s %s", action, strbuf_str(&sb));
        strbuf_free(&sb);
    }
}

/**
 * Creates a table with m buckets, and inserts entries[0..n-1] in it (in this order), tracing these actions.
 * NB: it uses provided functions only (not hashtable_init, an exercise): the setup of a test never depends on the
 * exercises.
 */
static HashTable* make_table(const char* name, int m, const TInfo* entries, int n) {
    if(trace_on()) {
        char action[64];
        snprintf(action, sizeof(action), "%s = hashtable_create(%d), then inserting", name, m);
        trace_action_with_entries(action, entries, n);
    }
    HashTable* h = hashtable_create(m);
    for(int i = 0; i < n; i++) {
        hashtable_insert(h, entries[i].key, entries[i].value);
    }
    trace_table(name, h);
    return h;
}

/* Checks on hash tables */

// Returns 1 iff the keys a and b are equal (as for tinfo_equal), 0 otherwise
static int same_key(TKey a, TKey b) {
    return tinfo_equal((TInfo){ .key = a }, (TInfo){ .key = b });
}

// Returns the node of h holding key, searching all the buckets (unlike hashtable_search, which searches only
// the bucket key hashes to), or NULL if there is none
static list* find_in_any_bucket(HashTable* h, TKey key) {
    for(int i = 0; i < h->nbuckets; i++) {
        for(list* l = h->bucket[i]; l != NULL; l = l->next) {
            if(same_key(l->val.key, key)) {
                return l;
            }
        }
    }
    return NULL;
}

// Writes into problem the first difference between h and expected[0..n-1], or the first broken invariant, if any
static void find_table_problem(HashTable* h, const TInfo* expected, int n, StrBuf* problem) {
    if(h == NULL) {
        strbuf_printf(problem, "the table is NULL");
        return;
    }
    // each entry of h is in the bucket its key hashes to, its key occurs once, and it is expected
    for(int i = 0; i < h->nbuckets; i++) {
        for(list* l = h->bucket[i]; l != NULL; l = l->next) {
            const TInfo e = l->val;
            const unsigned int hash = hashtable_hash(h, e.key);
            int j = 0; // position of e.key in expected, if any (n otherwise)
            while(j < n && !same_key(expected[j].key, e.key)) {
                j++;
            }
            if(hash != (unsigned int)i) {
                strbuf_printf(problem, "key " TKEY_FMT " is in bucket %d, but it hashes to %u", e.key, i, hash);
            } else if(list_search(l->next, e) != NULL) { // NB: if it occurs twice, the other one is in the same chain
                strbuf_printf(problem, "key " TKEY_FMT " occurs more than once", e.key);
            } else if(j == n) {
                strbuf_printf(problem, "unexpected entry ");
                append_entry(problem, e);
            } else if(e.value != expected[j].value) {
                strbuf_printf(problem, "key " TKEY_FMT " has value " TVALUE_FMT ", expected " TVALUE_FMT,
                              e.key, e.value, expected[j].value);
            }
            if(problem->len > 0) {
                return;
            }
        }
    }
    // each expected entry is in h
    for(int j = 0; j < n; j++) {
        if(find_in_any_bucket(h, expected[j].key) == NULL) {
            strbuf_printf(problem, "missing entry ");
            append_entry(problem, expected[j]);
            return;
        }
    }
    // so, h holds n entries: its size must be n
    if(h->size != n) {
        strbuf_printf(problem, "size is %d, but the table holds %d entries", h->size, n);
    }
}

static int check_table(HashTable* h, const TInfo* expected, int n, const char* file, int line, const char* name) {
    StrBuf problem = STRBUF_INIT, what = STRBUF_INIT, actual = STRBUF_INIT, expected_str = STRBUF_INIT;
    find_table_problem(h, expected, n, &problem);
    const int ok = problem.len == 0;
    strbuf_printf(&what, "content of %s%s%s", name, ok ? "" : ": ", strbuf_str(&problem));
    append_table(&actual, h);
    append_entries(&expected_str, expected, n);
    check_result(ok, file, line, strbuf_str(&what), strbuf_str(&actual), strbuf_str(&expected_str));
    strbuf_free(&problem);
    strbuf_free(&what);
    strbuf_free(&actual);
    strbuf_free(&expected_str);
    return ok;
}
// NB: expected must be an array (or NULL, if n is 0), e.g.: const TInfo expected[] = {{1, 10}, {2, 20}};
#define CHECK_TABLE(h, expected, n) check_table((h), (expected), (n), __FILE__, __LINE__, #h)

// Checks the result of hashtable_search(h, key): found with value expected_value (if expect_found), or not found
static int check_search(HashTable* h, TKey key, int expect_found, TValue expected_value,
                        const char* file, int line, const char* name) {
    StrBuf what = STRBUF_INIT, actual = STRBUF_INIT, expected = STRBUF_INIT;
    strbuf_printf(&what, "hashtable_search(%s, " TKEY_FMT ")", name, key);
    const TValue* v = hashtable_search(h, key);
    if(v != NULL) {
        strbuf_printf(&actual, "found " TVALUE_FMT, *v);
    } else {
        strbuf_printf(&actual, "not found");
    }
    if(expect_found) {
        strbuf_printf(&expected, "found " TVALUE_FMT, expected_value);
    } else {
        strbuf_printf(&expected, "not found");
    }
    const int ok = expect_found ? v != NULL && *v == expected_value : v == NULL;
    check_result(ok, file, line, strbuf_str(&what), strbuf_str(&actual), strbuf_str(&expected));
    strbuf_free(&what);
    strbuf_free(&actual);
    strbuf_free(&expected);
    return ok;
}
#define CHECK_FOUND(h, key, value) check_search((h), (key), 1, (value), __FILE__, __LINE__, #h)
#define CHECK_NOT_FOUND(h, key) check_search((h), (key), 0, 0, __FILE__, __LINE__, #h)

// Checks that the chain of bucket i of h holds exactly expected[0..n-1], in this order (from the head)
static int check_chain(HashTable* h, int i, const TInfo* expected, int n, const char* file, int line,
                       const char* name) {
    StrBuf what = STRBUF_INIT, actual = STRBUF_INIT, expected_str = STRBUF_INIT;
    strbuf_printf(&what, "chain of bucket %d of %s", i, name);
    int ok = 0;
    if(h == NULL || i < 0 || i >= h->nbuckets) {
        strbuf_printf(&actual, "no bucket %d", i);
    } else {
        const list* l = h->bucket[i];
        append_chain(&actual, l);
        int j = 0;
        while(l != NULL && j < n && same_key(l->val.key, expected[j].key) && l->val.value == expected[j].value) {
            l = l->next;
            j++;
        }
        ok = l == NULL && j == n;
    }
    append_chain_of_entries(&expected_str, expected, n);
    check_result(ok, file, line, strbuf_str(&what), strbuf_str(&actual), strbuf_str(&expected_str));
    strbuf_free(&what);
    strbuf_free(&actual);
    strbuf_free(&expected_str);
    return ok;
}
#define CHECK_CHAIN(h, i, expected, n) check_chain((h), (i), (expected), (n), __FILE__, __LINE__, #h)

/* Tests */

// The entries of several tests: with h(k) = k mod 10, 4 and 44 collide in bucket 4, and 85 and 175 in bucket 5
static const TInfo ENTRIES[] = { {4, 77}, {44, 66}, {85, 99}, {175, 55} };
enum { NUM_ENTRIES = (int)(sizeof(ENTRIES) / sizeof(ENTRIES[0])) };

static void test_create(void) {
    trace("[ACT] h = hashtable_create(10)");
    HashTable* h = hashtable_create(10);
    trace_table("h", h);
    if(!CHECK(h != NULL)) {
        return;
    }
    CHECK_INT(h->nbuckets, 10);
    CHECK_TABLE(h, NULL, 0);
    hashtable_destroy(h);
}

static void test_insert_and_search(void) {
    HashTable* h = make_table("h", 10, ENTRIES, NUM_ENTRIES);
    CHECK_TABLE(h, ENTRIES, NUM_ENTRIES);
    for(int i = 0; i < NUM_ENTRIES; i++) {
        CHECK_FOUND(h, ENTRIES[i].key, ENTRIES[i].value);
    }
    // unsuccessful searches: in an empty bucket (9), and in a non-empty one (4), whose chain is scanned to its end
    CHECK_NOT_FOUND(h, 179);
    CHECK_NOT_FOUND(h, 14);
    // negative keys: -3 mod 10 = 7 (while, in C, -3 % 10 == -3); so -3 and -13 collide in bucket 7
    CHECK_INT(hashtable_hash(h, -3), 7);
    CHECK_INT(hashtable_hash(h, -13), 7);
    trace("[ACT] inserting -3->1 and -13->2 (negative keys)");
    hashtable_insert(h, -3, 1);
    hashtable_insert(h, -13, 2);
    trace_table("h", h);
    CHECK_FOUND(h, -3, 1);
    CHECK_FOUND(h, -13, 2);
    const TInfo expected[] = { {4, 77}, {44, 66}, {85, 99}, {175, 55}, {-3, 1}, {-13, 2} };
    CHECK_TABLE(h, expected, 6);
    hashtable_destroy(h);
}

static void test_collisions(void) {
    HashTable* h = make_table("h", 10, ENTRIES, NUM_ENTRIES);
    // with h(k) = k mod 10, 4 and 44 collide in bucket 4, and 85 and 175 in bucket 5
    CHECK_INT(hashtable_hash(h, 4), 4);
    CHECK_INT(hashtable_hash(h, 44), 4);
    CHECK_INT(hashtable_hash(h, 85), 5);
    CHECK_INT(hashtable_hash(h, 175), 5);
    // a new entry is inserted at the head of its chain (as CHAINED-HASH-INSERT in CLRS): the last inserted comes first
    const TInfo chain4[] = { {44, 66}, {4, 77} };
    const TInfo chain5[] = { {175, 55}, {85, 99} };
    CHECK_CHAIN(h, 4, chain4, 2);
    CHECK_CHAIN(h, 5, chain5, 2);
    CHECK_CHAIN(h, 0, NULL, 0);
    hashtable_destroy(h);
}

static void test_insert_existing_key(void) {
    HashTable* h = make_table("h", 10, ENTRIES, NUM_ENTRIES);
    trace("[ACT] inserting 175->88 (175 is already present)");
    hashtable_insert(h, 175, 88);
    trace_table("h", h);
    CHECK_FOUND(h, 175, 88);
    // the value is updated, with no new entry
    const TInfo expected[] = { {4, 77}, {44, 66}, {85, 99}, {175, 88} };
    CHECK_TABLE(h, expected, 4);
    hashtable_destroy(h);
}

static void test_search_value(void) {
    HashTable* h = make_table("h", 10, ENTRIES, NUM_ENTRIES);
    CHECK_INT(hashtable_search_value(h, 55), 1);
    CHECK_INT(hashtable_search_value(h, 371), 0);
    trace("[ACT] inserting 175->88 (175 is already present)");
    hashtable_insert(h, 175, 88);
    trace_table("h", h);
    CHECK_INT(hashtable_search_value(h, 55), 0);
    CHECK_INT(hashtable_search_value(h, 88), 1);
    hashtable_destroy(h);
}

static void test_delete(void) {
    // the chain of bucket 4 is (84->1 44->66 4->77)
    const TInfo entries[] = { {4, 77}, {44, 66}, {84, 1}, {85, 99}, {175, 55} };
    HashTable* h = make_table("h", 10, entries, 5);

    trace("[ACT] deleting 44 (in the middle of its chain)");
    hashtable_delete(h, 44);
    trace_table("h", h);
    const TInfo after_44[] = { {4, 77}, {84, 1}, {85, 99}, {175, 55} };
    CHECK_TABLE(h, after_44, 4);

    trace("[ACT] deleting 84 (at the head of its chain)");
    hashtable_delete(h, 84);
    trace_table("h", h);
    const TInfo after_84[] = { {4, 77}, {85, 99}, {175, 55} };
    CHECK_TABLE(h, after_84, 3);

    trace("[ACT] deleting 4 (the only entry of its chain)");
    hashtable_delete(h, 4);
    trace_table("h", h);
    const TInfo after_4[] = { {85, 99}, {175, 55} };
    CHECK_TABLE(h, after_4, 2);
    CHECK_CHAIN(h, 4, NULL, 0);

    trace("[ACT] deleting 14 and 95 (not present; their buckets, 4 and 5, are empty and not)");
    hashtable_delete(h, 14);
    hashtable_delete(h, 95);
    trace_table("h", h);
    CHECK_TABLE(h, after_4, 2);
    hashtable_destroy(h);
}

// Returns how many of the keys first, first + step, ... (up to last) h holds, each with value 10 * key
static int count_present_10k(HashTable* h, int first, int last, int step) {
    int count = 0;
    for(int k = first; k <= last; k += step) {
        const TValue* v = hashtable_search(h, k);
        if(v != NULL && *v == 10 * k) {
            count++;
        }
    }
    return count;
}

// Adds to the trace the lengths of the chains of h (the shortest and the longest) and its load factor
static void trace_chain_lengths(const char* name, HashTable* h) {
    if(!trace_on() || h == NULL) {
        return;
    }
    int min = -1, max = -1;
    for(int i = 0; i < h->nbuckets; i++) {
        const int len = list_length(h->bucket[i]);
        min = min < 0 || len < min ? len : min;
        max = len > max ? len : max;
    }
    trace("%s: %d entries in %d buckets (load factor %.2f), chains of %d to %d entries", name, count_entries(h),
          h->nbuckets, (double)count_entries(h) / h->nbuckets, min, max);
}

static void test_many_entries(void) {
    enum { N = 1000, M = 7 };
    trace("[ACT] h = hashtable_create(%d), then inserting k->10k for k = 0, ..., %d", M, N - 1);
    HashTable* h = hashtable_create(M);
    for(int k = 0; k < N; k++) {
        hashtable_insert(h, k, 10 * k);
    }
    trace_chain_lengths("h", h);
    // NB: if h holds N entries, and all the N keys are found, the invariants hold too (no key occurs twice, and
    // each key is in the bucket it hashes to, the only one searched by hashtable_search)
    CHECK_INT(count_entries(h), N);
    CHECK_INT(count_present_10k(h, 0, N - 1, 1), N);

    trace("[ACT] deleting the even keys");
    for(int k = 0; k < N; k += 2) {
        hashtable_delete(h, k);
    }
    trace_chain_lengths("h", h);
    CHECK_INT(count_entries(h), N / 2);
    CHECK_INT(count_present_10k(h, 1, N - 1, 2), N / 2); // the odd keys
    CHECK_INT(count_present_10k(h, 0, N - 1, 2), 0);     // the even keys
    hashtable_destroy(h);
}

static void test_init(void) {
    // TODO: test hashtable_init, e.g. creating a table with some entries and checking its content (CHECK_TABLE);
    // see the other tests, then remove the following line
    TEST_TODO("write the test of hashtable_init");
}

static void test_merge(void) {
    // TODO: test hashtable_merge, e.g. merging two tables with some entries and checking the content of the result
    // (CHECK_TABLE), and that the two tables are unchanged; see the other tests, then remove the following line
    TEST_TODO("write the test of hashtable_merge");
}

static const TestCase TESTS[] = {
    TEST_CASE(test_create, "create an empty table"),
    TEST_CASE(test_insert_and_search, "insert some entries (also with negative keys), then search keys present and not"),
    TEST_CASE(test_collisions, "colliding keys are chained in the same bucket, the last inserted first"),
    TEST_CASE(test_insert_existing_key, "inserting a key already present updates its value"),
    TEST_CASE(test_search_value, "search values present and not, also after the value of a key is updated"),
    TEST_CASE(test_delete, "delete keys in the middle, at the head and at the end of their chains, and keys not present"),
    TEST_CASE(test_many_entries, "insert, search and delete many entries (long chains)"),
    TEST_CASE(test_init, "hashtable_init (exercise)"),
    TEST_CASE(test_merge, "hashtable_merge (exercise)"),
};
enum { NUM_TESTS = (int)(sizeof(TESTS) / sizeof(TESTS[0])) };

int hashtable_run_tests(const char* suite, int verbosity, int* num_failed) {
    *num_failed = run_tests(suite, TESTS, NUM_TESTS, verbosity);
    return NUM_TESTS;
}
