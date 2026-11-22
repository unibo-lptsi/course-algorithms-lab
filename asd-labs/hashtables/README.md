# Lab `hashtables`: tabelle hash

## Esercizi

1. [Tempo stimato: 30'] **Hashtable a indirizzamento chiuso (chained)**
    - NB sulla terminologia: *indirizzamento chiuso* = *chaining* (le collisioni restano nel bucket, in una lista), *indirizzamento aperto* = *open addressing*; alcuni testi però chiamano il chaining *open hashing* e l'indirizzamento aperto *closed hashing*.
    - **Studiare** i sorgenti (si veda la sezione [Sorgenti e compilazione](#sorgenti-e-compilazione)), in particolare:
        - si compili ed **esegua** il programma, e si cerchi di comprendere l'output: per ogni test fallito vengono stampati i controlli falliti (con valore effettivo e atteso) e alla fine un riepilogo; inizialmente due test falliscono, perché sono **ancora da scrivere** (si veda sotto)
        - si riesegua il programma con un livello di **verbosità** maggiore (si veda la sezione [Test e report](#test-e-report)), ad es. `./hashtable 5`, e si osservino le tabelle mostrate nella traccia dei test, nella forma `[n/m]{[i](catena del bucket i) ...}`, con `n` entry e `m` bucket (quindi `n/m` è il *fattore di carico*), ad es. `[4/10]{[4](44->66 4->77) [5](175->55 85->99)}`: quali chiavi collidono? In che ordine compaiono nella catena?
        - NB: i test usano `m = 10` per rendere evidenti le collisioni (con `h(k) = k mod 10` conta solo l'ultima cifra decimale di `k`), ma in generale è una scelta cattiva: con il metodo della divisione conviene un `m` primo, non vicino a una potenza di 2. Cosa succederebbe con `m = 7`?
        - si studi poi l'interfaccia `hashtable.h` e l'implementazione `hashtable.c`: ogni bucket `bucket[i]` contiene la **catena** (una lista collegata, del modulo `list`) delle entry la cui chiave ha hash `i`; si notino gli **invarianti** indicati in `hashtable.h`
    - **Implementare in `hashtable.c`** le seguenti funzioni (già dichiarate in `hashtable.h`):
        - `HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries)`, che crei e inizializzi una hashtable con le entry fornite
        - `HashTable *hashtable_merge(HashTable* h1, HashTable *h2)`, che restituisca una nuova hashtable data dall'unione delle due tabelle hash fornite in input (per le chiavi presenti in entrambe prevale il valore in `h2`; `h1` e `h2` non vanno modificate)
    - **Si scrivano i test** per verificare le funzionalità implementate: in `hashtable_test.c`, si completino `test_init` e `test_merge` (sostituendo `TEST_TODO(...)`) prendendo spunto dagli altri test; ad es., `CHECK_TABLE(h, expected, n)` verifica che la tabella `h` contenga esattamente le entry `expected[0..n-1]` (in qualsiasi ordine) e che ne siano rispettati gli invarianti.
2. [Tempo stimato: 30'] **Chiavi stringa**. Si copino tutti i sorgenti in una nuova cartella `string-keys/` e si vadano ad adattare per utilizzare il tipo stringa `char*` per le chiavi.
    - Occorre modificare la `typedef` di `TKey` (e il formato `TKEY_FMT`), aggiustare implementazioni di funzioni (dove si confrontano le chiavi?), definire una nuova funzione di `hash` (si faccia riferimento alle slide di teoria per un esempio di implementazione; per provarne più d'una, si veda `hashtable_set_hash_function`) e adattare i test (con chiavi stringa).
    - Suggerimento: nei test, si cerchi una chiave presente nella tabella passandone una *copia* (ad es. `char copy[] = "foo";`): viene trovata?
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

- Ogni test è una funzione `test_*` di `hashtable_test.c`, elencata nell'array `TESTS`; verifica i risultati delle operazioni con dei **controlli**: quelli generici `CHECK(condizione)` e `CHECK_INT(valore, atteso)`, e quelli per le tabelle hash `CHECK_TABLE` (contenuto e invarianti), `CHECK_FOUND` e `CHECK_NOT_FOUND` (esito di `hashtable_search`), `CHECK_CHAIN` (catena di un bucket).
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
