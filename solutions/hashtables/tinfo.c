/**
 * Module tinfo: key-value pairs (implementation). The interface is in tinfo.h.
 */
#include "tinfo.h" // first, so that the compiler checks the header does not depend on previous includes

#include <stdio.h>

int tinfo_equal(TInfo a, TInfo b) {
    return a.key == b.key /* && a.value == b.value */; // NB: equality only based on keys
}

int tinfo_print(TInfo info) {
    // NB: printing directly needs no buffer, which could be too small (e.g., for a long string key)
    return printf(TKEY_FMT "->" TVALUE_FMT, info.key, info.value);
}
