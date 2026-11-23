# Lab `hashtables`: tabelle hash

## Esercizi

1. [Tempo stimato: 45'] **Hashtable a indirizzamento chiuso (chained)**
    - NB sulla terminologia: *indirizzamento chiuso* = *chaining* (le collisioni restano nel bucket, in una lista), *indirizzamento aperto* = *open addressing*; alcuni testi però chiamano il chaining *open hashing* e l'indirizzamento aperto *closed hashing*.
    - **Studiare** i sorgenti (si veda la sezione [Sorgenti e compilazione](#sorgenti-e-compilazione)), in particolare:
        - si compili ed **esegua** il programma, e si cerchi di comprendere l'output: per ogni test fallito vengono stampati i controlli falliti (con valore effettivo e atteso) e alla fine un riepilogo; inizialmente quasi tutti i test falliscono: la maggior parte perché usa funzioni **ancora da implementare** (`hashtable_search`, `hashtable_insert`, ..., si veda sotto), `test_init` e `test_merge` perché sono **ancora da scrivere**, `test_rehash` e `test_dynamic_table` perché riguardano l'esercizio 2
        - si riesegua il programma con un livello di **verbosità** maggiore (si veda la sezione [Test e report](#test-e-report)), ad es. `./hashtable 5`, e si osservino le tabelle mostrate nella traccia dei test, nella forma `[n/m]{[i](catena del bucket i) ...}`, con `n` entry e `m` bucket (quindi `n/m` è il *fattore di carico*), ad es. `[4/10]{[4](44->66 4->77) [5](175->55 85->99)}`: quali chiavi collidono? In che ordine compaiono nella catena?
        - NB: i test usano `m = 10` per rendere evidenti le collisioni (con `h(k) = k mod 10` conta solo l'ultima cifra decimale di `k`), ma in generale è una scelta cattiva: con il metodo della divisione conviene un `m` primo, non vicino a una potenza di 2. Cosa succederebbe con `m = 7`?
        - si studi poi l'interfaccia `hashtable.h` e l'implementazione `hashtable.c`: ogni bucket `bucket[i]` contiene la **catena** (una lista collegata, del modulo `list`) delle entry la cui chiave ha hash `i`; si notino gli **invarianti** indicati in `hashtable.h`
    - **Implementare in `hashtable.c`** le seguenti funzioni (già dichiarate in `hashtable.h`), nell'ordine:
        - `TValue *hashtable_search(HashTable* h, TKey key)`, che restituisca il **puntatore al valore** associato a `key`, cioè al campo `value` del nodo della catena che contiene l'entry con quella chiave, o `NULL` se `key` non è presente: si calcoli il bucket della chiave e se ne scorra la catena (CHAINED-HASH-SEARCH in CLRS). NB: il puntatore deve riferirsi al valore *nel nodo*, non a una sua copia (ad es. in una variabile locale): così chi chiama può anche modificare il valore, con `*hashtable_search(h, key) = v`
        - `void hashtable_insert(HashTable* h, TKey key, TValue val)`, che inserisca l'entry `key->val` **in testa** alla catena del suo bucket (CHAINED-HASH-INSERT in CLRS) o, se `key` è già presente, ne aggiorni il valore (così nessuna chiave compare due volte, si veda il commento in `hashtable.h`), mantenendo aggiornato `size`
        - entrambe accettano una tabella `NULL`, trattata come vuota (si veda `hashtable.h`); i loro test (`test_search`, `test_insert`, `test_insert_existing_key`) sono già scritti. NB: i test costruiscono le tabelle con `make_table` (in `hashtable_test.c`), che non usa `hashtable_insert`: così `test_search` si può superare anche prima di implementare `hashtable_insert`
        - implementate `hashtable_search` e `hashtable_insert`, passano anche `test_more_entries_than_buckets` e `test_many_entries`: se ne osservi la traccia (`./hashtable 5`). Una tabella con liste di trabocco **non si riempie mai**: può contenere più entry che bucket (fattore di carico maggiore di 1), a scapito della lunghezza delle catene, e quindi del costo atteso delle operazioni, Θ(1 + α) con α = n/m (in `test_many_entries`, 1000 entry in 7 bucket)
        - `void hashtable_add_many(HashTable* h, const TInfo* entries, int nentries)`, che inserisca in `h` le entry `entries[0..nentries-1]`, nell'ordine, con `hashtable_insert` (quindi, se una chiave compare più volte, prevale l'ultimo valore), e `HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries)`, che crei e inizializzi una hashtable con le entry fornite (con `hashtable_add_many`)
        - `HashTable *hashtable_merge(HashTable* h1, HashTable *h2)`, che restituisca una nuova hashtable data dall'unione delle due tabelle hash fornite in input (per le chiavi presenti in entrambe prevale il valore in `h2`; `h1` e `h2` non vanno modificate)
    - **Si scrivano i test** di `hashtable_init` e `hashtable_add_many` (in `test_init`) e di `hashtable_merge` (in `test_merge`): in `hashtable_test.c`, si completino i due test (sostituendo `TEST_TODO(...)`) prendendo spunto dagli altri test; ad es., `CHECK_TABLE(h, expected, n)` verifica che la tabella `h` contenga esattamente le entry `expected[0..n-1]` (in qualsiasi ordine) e che ne siano rispettati gli invarianti.
2. [EXTRA - Tempo stimato: 45'] **Espansione e rehashing**. Una tabella creata con `hashtable_create` è *fissa*: il numero di bucket `m` non cambia mai, quindi il fattore di carico α = n/m cresce con il numero di entry `n`, e con esso la lunghezza delle catene e il costo atteso delle operazioni. Una tabella *dinamica*, creata con `hashtable_create_dynamic(m, max_load_factor)`, invece si **espande**: quando un inserimento porta il fattore di carico oltre `max_load_factor`, il numero di bucket viene circa raddoppiato e le entry vengono ridistribuite nei nuovi bucket (*rehashing*), così α resta limitato.
    - **Implementare in `hashtable.c`**:
        - `void hashtable_rehash(HashTable* h, int new_nbuckets)`, che sposti le entry di `h` in un nuovo array di `new_nbuckets` bucket, ciascuna nel bucket in cui va la sua chiave **con il nuovo numero di bucket**, e liberi il vecchio array. Non si allochino nuovi nodi: si **ricolleghino** quelli esistenti (modificandone il campo `next`), così gli indirizzi restituiti da `hashtable_search` restano validi (si veda `hashtable.h`)
        - l'**espansione**, in `hashtable_insert` (si veda il `TODO`): se `h` è dinamica (`max_load_factor > 0`) e, dopo l'inserimento di una nuova entry, `size > max_load_factor * nbuckets`, si porti il numero di bucket al più piccolo numero primo `>= 2 * nbuckets` (con il metodo della divisione conviene un `m` primo, si veda l'esercizio 1), ad es. con una funzione ausiliaria `static int next_prime(int n)`
    - I test `test_rehash` e `test_dynamic_table`, già scritti, verificano le due funzionalità. Il secondo inserisce le stesse 1000 chiavi in una tabella fissa e in una dinamica (con fattore di carico massimo 2), entrambe con 7 bucket iniziali, e confronta la lunghezza della catena più lunga: si osservi la traccia (`./hashtable 5`), in cui compaiono anche le espansioni della tabella dinamica.
    - **Domande**: perché non basta copiare le catene bucket per bucket in un array più grande (o ingrandire l'array con `realloc`, come in `darray`)? Perché si raddoppia il numero di bucket, invece di aumentarlo di una costante (ad es. `m + 10`), e qual è quindi il costo ammortizzato di un inserimento? (Si ricordino le politiche lineare e geometrica del lab `dynamic-arrays`.) Infine, perché con le espansioni sarebbe opportuno che `HashTable` fosse un tipo opaco, come `DArray`?
3. [EXTRA - Tempo stimato: 30'] Esercizio di realtà: esplorare l'implementazione della classe [java.util.HashMap](https://github.com/openjdk/jdk/blob/master/src/java.base/share/classes/java/util/HashMap.java) e cercare di ritrovare concetti introdotti a lezione
4. [EXTRA - Tempo stimato: 120'] Prendendo spunto da `hashtable.c`, implementare una hashtable a indirizzamento aperto.
    - Si ricorda che una hashtable a indirizzamento aperto memorizza le entry direttamente nei bucket e risolve le collisioni occupando altri bucket della tabella, secondo una *sequenza di ispezione* (ad es. quelli successivi: *linear probing*; si vedano anche *quadratic probing* e *double hashing*).


## Sorgenti e compilazione

Il codice è organizzato in **moduli** (come nel lab `dynamic-arrays`): ogni modulo ha un header (`.h`), che ne dichiara l'interfaccia, e un file di implementazione (`.c`).

| File | Contenuto |
|---|---|
| `tinfo.h`, `tinfo.c` | modulo `tinfo`: i tipi delle chiavi (`TKey`), dei valori (`TValue`) e delle entry (`TInfo`, coppie chiave-valore); confronto (per chiave) e stampa delle entry |
| `list.h`, `list.c` | modulo `list`: liste collegate di `TInfo`, usate come catene dei bucket |
| `hashtable.h`, `hashtable.c` | modulo `hashtable`: tabelle hash con liste di trabocco (chaining), comprese le funzioni da completare |
| `test_harness.h`, `test_harness.c` | modulo `test_harness`: un semplice *harness* di test (esecuzione dei test, controlli, report), indipendente dalle tabelle hash |
| `hashtable_test.h`, `hashtable_test.c` | modulo `hashtable_test`: i test del modulo `hashtable`, compresi quelli da scrivere |
| `main.c` | programma principale: esegue i test |

Le dipendenze tra i moduli seguono gli `#include`: `hashtable` usa `list`, ed entrambi usano `tinfo`; `hashtable_test` usa `hashtable` e `test_harness`.

**Compilazione**: si passano a `gcc` tutti i file `.c`:

```sh
gcc -Wall -o hashtable *.c
./hashtable         # su Windows: .\hashtable.exe
```

o, con la compilazione separata (si veda il lab `dynamic-arrays`):

```sh
gcc -Wall -c tinfo.c list.c hashtable.c test_harness.c hashtable_test.c main.c   # -> file .o
gcc -o hashtable tinfo.o list.o hashtable.o test_harness.o hashtable_test.o main.o
```

### Test e report

- Ogni test è una funzione `test_*` di `hashtable_test.c`, elencata nell'array `TESTS`; verifica i risultati delle operazioni con dei **controlli**: quelli generici `CHECK(condizione)`, `CHECK_INT(valore, atteso)` e `CHECK_AT_MOST(valore, massimo)`, e quelli per le tabelle hash `CHECK_TABLE` (contenuto e invarianti), `CHECK_FOUND` e `CHECK_NOT_FOUND` (esito di `hashtable_search`), `CHECK_CHAIN` (catena di un bucket).
- Un controllo fallito viene segnalato, con valore effettivo (`actual`) e atteso (`expected`), e il test prosegue (a differenza di `assert`, che termina il programma). Un test passa se esegue almeno un controllo e nessuno fallisce. Il programma termina con codice di uscita `0` solo se passano tutti i test.
- La **verbosità** del report si sceglie con un argomento del programma, `./hashtable N` (o modificando `default_verbosity` in `main.c`), dove `N` è la somma dei flag da attivare:

| `N` | Flag (`test_harness.h`) | Il report mostra |
|---|---|---|
| `0` | (nessuno: default) | solo i test falliti, ciascuno con i controlli falliti, e il riepilogo |
| `1` | `VERBOSE_PASSED_TESTS` | anche i test superati |
| `2` | `VERBOSE_ALL_CHECKS` | per ogni test mostrato, tutti i controlli (non solo quelli falliti), con valore effettivo e atteso |
| `4` | `VERBOSE_TRACE` | per ogni test mostrato, la traccia: le azioni eseguite (`[ACT]`) e le tabelle risultanti |

Ad es., `./hashtable 4` mostra la traccia dei soli test falliti, `./hashtable 5` tutti i test con la loro traccia, `./hashtable 7` tutto; `./hashtable help` riassume le opzioni.

- Se un test va in crash (ad es. per un accesso a memoria non valido, *segmentation fault*), il report indica quale test era in esecuzione (`[CRASH] ...`) prima che il programma termini.
