# Lab `search`: algoritmi di ricerca

**Razionale/obiettivi:** esercitarsi con gli **algoritmi di ricerca** approfonditi a lezione (ricerca lineare e binaria)

Svolgere i seguenti esercizi, leggendo attentamente le istruzioni.

### Parte 1

1. *[Tempo stimato: 90']* Implementare, testare, e misurare gli algoritmi `linear_search` (ricerca lineare) e `binary_search` (ricerca binaria/dicotomica). Ovvero, per ogni algoritmo:
    1. scrivere un'implementazione in Python
        - si può lavorare a partire dai file `search_linear_search.py` e `search_binary_search.py`
        - ATTENZIONE: occorre **evitare di procedere semplicemente traducendo lo pseudocodice dato verso un'implementazione Python** (questo approccio trasforma il problema dall'*implementare l'algoritmo* a *tradurre istruzioni di pseudocodice ad istruzioni del linguaggio di programmazione scelto*!!!). Il procedimento dovrebbe essere il seguente:
            1. prima di iniziare a scrivere codice, cercare di comprendere l'idea / principio dietro un algoritmo: si consiglia di partire esercitando l'algoritmo con carta e penna su esempi piccoli
            2. disattivando CoPilot o altri strumenti di code-completion, provare a implementare l'idea (N.B.: senza guardare lo pseudocodice fornito, oppure guardandolo una sola volta all'inizio)
            3. verificare la propria soluzione, e correggere in caso di errori
            4. se dopo un po' di tentativi ancora non vi è soluzione, dare un'occhiata allo pseudocodice fornito
    2. scrivere alcuni test per stimare la correttezza dell'implementazione
        - si parta dalla test suite parziale in `test_search_algorithms.py`, basata sul modulo `unittest` (cf. lab `testing`), e la si estenda con ulteriori casi di test
        - si noti come **la stessa suite di test serva per entrambi gli algoritmi**: i test comuni sono definiti nella classe `SearchTests`, mentre le classi `TestLinearSearch` e `TestBinarySearch` (sottoclassi di `SearchTests` e di `unittest.TestCase`) ereditano tali test e specificano solo l'algoritmo da eseguire (campo `search`), più eventuali test specifici
        - si eseguano i test con `python3 test_search_algorithms.py`
        - DOMANDE: quale precondizione devono rispettare gli input dei test comuni? Se l'elemento cercato compare più volte nell'array, che risultato ci aspettiamo?

### Parte 2

2. Con riferimento al file `measure_search_algorithms.py`, che fornisce uno scheletro da completare, **misurare e graficare i tempi d'esecuzione di `linear_search` e `binary_search`**
    1. completare la funzione `measure`, che misura i tempi d'esecuzione di un algoritmo su una lista di input `(array, x)` di dimensione crescente, usando la funzione `measure_ms` (già fornita, cf. lab `plotting`)
        - si notino i *type alias* `SearchAlgorithm`, `SearchInput` e `Measures` (cf. lab `testing`) usati nelle firme delle funzioni e nelle variabili: documentano la struttura dei dati
    2. definire gli input `worst_inputs` corrispondenti al caso peggiore: quale elemento `x` conviene cercare?
    3. completare la funzione `plot_times`, che grafica con matplotlib i tempi d'esecuzione nel caso peggiore al variare della dimensione dell'array, in due grafici affiancati: uno in scala lineare e uno in scala logaritmica (su entrambi gli assi)
        - la struttura della figura (assi, scale, titoli, legenda) è già fornita: mancano le chiamate a `plot()` che tracciano le curve. Si osservi il tipo del parametro `times`: come si ricavano le liste di ascisse e ordinate?
        - prima di farlo: cosa ci aspettiamo di vedere?
        - DOMANDA: in scala logaritmica su entrambi gli assi, una funzione del tipo `c * n^k` appare come una retta di pendenza `k`. Che andamento hanno le curve di `linear_search` e `binary_search`?
    4. (facoltativo) misurare i tempi anche con input di struttura diversa (ad es. elemento cercato in prima posizione, o in posizione casuale)
3. *[Tempo stimato: 30']* Implementare e testare `linear_search_rec` e `binary_search_rec`, varianti degli algoritmi di ricerca strutturati in modo ricorsivo
    - si può lavorare a partire dai file `search_linear_search.py` e `search_binary_search.py`, dove sono fornite le funzioni con corpo vuoto
    - per il testing, si aggiungano in `test_search_algorithms.py` le classi di test per le due varianti, riusando i test già scritti

