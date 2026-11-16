# Lab `dynamic-arrays`: array dinamici

## Esercizi

1. **Implementazione C di array dinamici**: Studio e integrazione funzionalità
    - **Studiare** i sorgenti, in particolare:
        - si compili ed **esegua** il programma (si veda sopra) e si cerchi di comprendere l'output: per ogni test viene stampato l'esito (`[PASS]` o `[FAIL]`, con i dettagli dei controlli falliti) e alla fine un riepilogo; inizialmente alcuni test falliscono perché sfruttano funzioni **ancora da implementare**.
        - si noti l'uso di una rappresentazione stringa `[size/capacity]{ a_0, ..., a_{n-1} }` per gli array dinamici 
        - si noti l'**invariante** della struttura dati: `0 <= size <= capacity`; le posizioni da `0` a `size-1` contengono gli elementi, quelle da `size` a `capacity-1` sono spazio libero
        - si noti, in `darray.c`, la differenza tra la macro **`REQUIRE`** e **`assert`**: entrambe, se la condizione è falsa, terminano subito il programma indicando file, riga e condizione; `REQUIRE` verifica le **precondizioni** delle funzioni pubbliche (ad es. una posizione valida), che deve garantire chi le chiama, mentre `assert` verifica i **controlli interni** (condizioni che solo un errore in `darray.c` può violare, come l'invariante dopo un ridimensionamento). Il motivo: compilando con `-DNDEBUG` (come si fa di solito per le versioni di rilascio di un programma) le `assert` vengono eliminate, e una precondizione violata ma non controllata porterebbe a corrompere la memoria (ad es. scrivendo fuori dal blocco allocato)
        - a questo punto si studi prima l'interfaccia `darray.h` e poi l'implementazione `darray.c`, e si osservino le funzioni rilevanti, tra cui: `darray_create_capac`, `darray_print`, `darray_realloc`, `darray_resize` (che delega alla politica di ridimensionamento in uso, `darray_resize_linear` o `darray_resize_geometric`), `darray_set_linear_policy`, `darray_set_geometric_policy`; e, in `darray_test.c`, `darray_assert_equals` (usata dai test per confrontare un array dinamico con un array "tradizionale")
        - si noti che `DArray` è un **tipo opaco**: `darray.h` lo dichiara (`typedef struct SDArray DArray;`), ma i suoi campi sono definiti solo in `darray.c`. Fuori da `darray.c`, quindi, un `DArray` si usa solo tramite puntatore (`DArray*`), restituito da `darray_create` e liberato da `darray_destroy` (che libera sia gli elementi sia il `DArray` stesso), e i suoi campi si leggono solo tramite funzioni (`darray_size`, `darray_capacity`, `darray_get`). Il motivo: ogni ridimensionamento può spostare in memoria gli elementi (`realloc`) e liberare il blocco precedente, a cui una copia della `struct` (`DArray b = *a;`) continuerebbe a puntare; con il tipo opaco una copia del genere non compila, mentre una copia del puntatore (`DArray* b = a;`) si riferisce allo stesso array e ne vede tutte le modifiche. Resta che, dopo `darray_destroy(a)`, né `a` né le sue copie vanno più usati
    - **Completare `darray.c` implementando le seguenti funzioni** (già dichiarate in `darray.h`):
        - **`darray_append`**: atta ad aggiungere alla fine dell'array un elemento (ridimensionando l'array)
        - **`darray_insert`**: atta ad aggiungere alla posizione indicata un nuovo elemento (traslando opportunamente quelli attualmente presenti); le posizioni valide vanno da `0` a `size` inclusa (inserire in posizione `size` equivale ad aggiungere in fondo)
        - **`darray_remove`**: atta a rimuovere l'elemento alla posizione indicata e a restituirlo (traslando opportunamente quelli successivi e ridimensionando l'array, il che può ridurne la capacità); le posizioni valide vanno da `0` a `size-1`
        - **`darray_init(size, value)`**: che crea un nuovo array dinamico inizializzando i `size` elementi al valore `value`
        - si verifichino le precondizioni (posizioni valide) con `REQUIRE`, come in `darray_set`
    - Si notino, in `darray_test.c`, le funzioni di test `test_*`, ciascuna dedicata a un'operazione, eseguite da `darray_run_tests()`, che `main` (in `main.c`) invoca con entrambe le politiche di ridimensionamento dell'array (lineare e geometrica)
        - a differenza di `REQUIRE` e `assert`, che terminano il programma alla prima violazione, i controlli dei test (`CHECK` e `darray_assert_equals`) segnalano il fallimento e lasciano proseguire: così il riepilogo finale elenca tutti i test che falliscono, e il programma termina con codice di uscita `0` solo se passano tutti. Se invece scatta un `REQUIRE` o un'`assert` (ad es. per un errore in una delle funzioni da implementare), il programma si interrompe subito, senza riepilogo: si legga il messaggio, che indica file, riga, funzione e condizione violata
        - il parametro `verbose` di `darray_run_tests` (impostato in `main`) stabilisce cosa riporta l'output: con `1` tutti i test, ciascuno con la sua traccia (operazioni eseguite e array risultanti); con `0` solo i test che falliscono, con i controlli falliti, e i riepiloghi
    - Nota: si compili con l'opzione **`-DDEBUG`** (si veda sopra) per attivare log di debug (stampati su `stderr`) e ispezionare il comportamento delle funzioni di riallocazione
    - **Domanda**: `darray_set_linear_policy` e `darray_set_geometric_policy` accettano solo parametri tali che `1 <= GROWING_DELTA < SHRINKING_DELTA` e `1 < GROWING_FACTOR < SHRINKING_FACTOR`. Perché? Cosa succederebbe, ad esempio, con `GROWING_FACTOR = 4` e `SHRINKING_FACTOR = 2` durante una sequenza di `darray_append`? (Suggerimento: si consideri la capacità subito dopo una riallocazione.)
2. *OPZIONALE.* Si consideri il notebook fornito **`plotting_darray_capacity_expansion.ipynb`**, che utilizza `matplotlib` per graficare come evolve la capacità (memoria allocata) di un array dinamico utilizzando le due tecniche di espansione (lineare e geometrica). 
    - Implementare la logica di espansione/contrazione lineare e geometrica, con le stesse regole di `darray_resize_linear` e `darray_resize_geometric`
    - Parametrizzare il codice rispetto ai delta, fattori, soglie di crescita/contrazione
    - Si cerchi di riprodurre una figura tipo la seguente

![](imgs/dynamic-array-capacity-growth.png)



## Sorgenti e compilazione

Il codice C è organizzato in **moduli**: ogni modulo ha un file *header* (`.h`), che ne dichiara l'**interfaccia** (i tipi e i prototipi delle funzioni che gli altri file possono usare), e un file di **implementazione** (`.c`), che contiene le definizioni delle funzioni.

| File | Contenuto |
|---|---|
| `darray.h` | interfaccia del modulo `darray`: tipo degli elementi `TInfo`, tipo (opaco) `DArray`, prototipi delle funzioni pubbliche |
| `darray.c` | implementazione del modulo `darray`: definizione di `struct SDArray` (i campi di `DArray`) e delle funzioni, comprese quelle da completare |
| `darray_test.h`, `darray_test.c` | modulo `darray_test`: i test del modulo `darray`, eseguiti da `darray_run_tests` |
| `main.c` | programma principale (`main`): esegue i test con entrambe le politiche di ridimensionamento |

- Un file che usa un modulo ne include l'header: ad es. `main.c` e `darray_test.c` contengono `#include "darray.h"` (con le virgolette, a differenza di `<stdio.h>`, il file viene cercato prima nella cartella del sorgente). Così conoscono i tipi e i prototipi delle funzioni, ma non la loro implementazione (né i campi di `DArray`, definiti in `darray.c`).
- Ogni header è protetto da una *include guard* (`#ifndef DARRAY_H` / `#define DARRAY_H` / ... / `#endif`), che evita di ripeterne il contenuto se lo stesso header viene incluso più volte, anche indirettamente, nello stesso file: alcune ripetizioni sarebbero errori (ad es. due definizioni della stessa `struct`).

**Compilazione in un solo comando**: si passano a `gcc` **tutti i file `.c`** (gli header no: vengono inclusi dai `.c`):

```sh
gcc -Wall -o darray main.c darray.c darray_test.c
./darray            # su Windows: .\darray.exe
```

o anche, più brevemente

```sh
gcc -Wall *.c
./a.out
```

**Compilazione separata**, ovvero ciò che il comando precedente fa dietro le quinte: ogni file `.c`, insieme agli header che include, è una *unità di compilazione*, compilata indipendentemente dalle altre in un *file oggetto* (`.o`) con l'opzione `-c`; poi il *linker* unisce i file oggetto in un eseguibile, collegando ogni chiamata di funzione (ad es. a `darray_append`, in `darray_test.o`) alla sua definizione (in `darray.o`):

```sh
gcc -Wall -c darray.c           # -> darray.o
gcc -Wall -c darray_test.c      # -> darray_test.o
gcc -Wall -c main.c             # -> main.o
gcc -o darray main.o darray.o darray_test.o     # linking -> darray (darray.exe su Windows)
```

- Dopo una modifica di `darray.c` basta ricompilare `darray.c` e ripetere il linking; se invece si modifica un header, vanno ricompilati tutti i `.c` che lo includono (strumenti come `make` automatizzano queste dipendenze).
- Se si dimentica un file (ad es. `gcc -Wall main.c darray_test.c`), ogni file viene compilato senza errori, ma il linking fallisce con errori del tipo `` undefined reference to ...``: le funzioni sono dichiarate (nell'header) ma non sono definite in nessuno dei file oggetto.

**Opzioni utili**:

- **`-DDEBUG`** definisce la macro `DEBUG`, che attiva i messaggi di log di `darray.c` (stampati su `stderr`) per ispezionare il comportamento delle funzioni di riallocazione: `gcc -Wall -DDEBUG -o darray main.c darray.c darray_test.c`. Una macro definita con `-D` vale solo per i file compilati con quel comando: con la compilazione separata, l'opzione va data nella compilazione di `darray.c` (`gcc -Wall -DDEBUG -c darray.c`), seguita dal linking.
- **`-DNDEBUG`** elimina le `assert`, ma non i `REQUIRE` di `darray.c` (si veda sopra): `gcc -Wall -DNDEBUG -o darray main.c darray.c darray_test.c`.
- *(Linux e macOS)* **`-g -fsanitize=address,undefined`** aggiunge al programma dei controlli eseguiti a runtime, che lo interrompono con un messaggio dettagliato in caso di accessi fuori dai limiti di un'area di memoria, uso di memoria già liberata, ecc.: utile per trovare errori nelle funzioni da implementare. Ad es.: `gcc -Wall -g -fsanitize=address,undefined -o darray main.c darray.c darray_test.c`.